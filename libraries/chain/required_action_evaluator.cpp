#include <freezone/chain/freezone_fwd.hpp>
#include <freezone/chain/required_action_evaluator.hpp>
#include <freezone/chain/database.hpp>
#include <freezone/chain/SST_objects.hpp>
#include <freezone/chain/util/SST_token.hpp>
#include <cmath>

namespace freezone { namespace chain {

#ifdef IS_TEST_NET

void example_required_evaluator::do_apply( const example_required_action& a ) {}

#endif

void SST_ico_launch_evaluator::do_apply( const SST_ico_launch_action& a )
{
   const SST_token_object& token = _db.get< SST_token_object, by_symbol >( a.symbol );
   const SST_ico_object& ico = _db.get< SST_ico_object, by_symbol >( token.liquid_symbol );

   _db.modify( token, []( SST_token_object& o )
   {
      o.phase = SST_phase::ico;
   } );

   SST_ico_evaluation_action eval_action;
   eval_action.control_account = token.control_account;
   eval_action.symbol = token.liquid_symbol;
   _db.push_required_action( eval_action, ico.contribution_end_time );
}

void SST_ico_evaluation_evaluator::do_apply( const SST_ico_evaluation_action& a )
{
   const SST_token_object& token = _db.get< SST_token_object, by_symbol >( a.symbol );
   const SST_ico_object& ico = _db.get< SST_ico_object, by_symbol >( token.liquid_symbol );

   if ( ico.contributed.amount >= ico.freezone_satoshi_min )
   {
      _db.modify( token, []( SST_token_object& o )
      {
         o.phase = SST_phase::ico_completed;
      } );

      SST_token_launch_action launch_action;
      launch_action.control_account = token.control_account;
      launch_action.symbol = token.liquid_symbol;
      _db.push_required_action( launch_action, ico.launch_time );
   }
   else
   {
      _db.modify( token, []( SST_token_object& o )
      {
         o.phase = SST_phase::launch_failed;
      } );

      if ( !util::SST::ico::schedule_next_refund( _db, token.liquid_symbol ) )
         _db.remove( ico );
   }
}

void SST_token_launch_evaluator::do_apply( const SST_token_launch_action& a )
{
   const SST_token_object& token = _db.get< SST_token_object, by_symbol >( a.symbol );

   _db.modify( token, []( SST_token_object& o )
   {
      o.phase = SST_phase::launch_success;
   } );

   if ( !util::SST::ico::schedule_next_contributor_payout( _db, token.liquid_symbol ) )
      util::SST::ico::remove_ico_objects( _db, token.liquid_symbol );
}

void SST_refund_evaluator::do_apply( const SST_refund_action& a )
{
   using namespace freezone::chain::util;

   _db.adjust_balance( a.contributor, a.refund );

   _db.modify( _db.get< SST_ico_object, by_symbol >( a.symbol ), [&]( SST_ico_object& o )
   {
      o.processed_contributions += a.refund.amount;
   } );

   auto key = boost::make_tuple( a.symbol, a.contributor, a.contribution_id );
   _db.remove( _db.get< SST_contribution_object, by_symbol_contributor >( key ) );

   if ( !SST::ico::schedule_next_refund( _db, a.symbol ) )
      util::SST::ico::remove_ico_objects( _db, a.symbol );
}

void SST_contributor_payout_evaluator::do_apply( const SST_contributor_payout_action& a )
{
   using namespace freezone::chain::util;

   share_type additional_token_supply = SST::ico::payout( _db, a.symbol, _db.get_account( a.contributor ), a.payouts );

   if ( additional_token_supply > 0 )
      _db.adjust_supply( asset( additional_token_supply, a.symbol ) );

   _db.modify( _db.get< SST_ico_object, by_symbol >( a.symbol ), [&]( SST_ico_object& ico )
   {
      ico.processed_contributions += a.contribution.amount;
   } );

   auto key = boost::make_tuple( a.symbol, a.contributor, a.contribution_id );
   _db.remove( _db.get< SST_contribution_object, by_symbol_contributor >( key ) );

   if ( !SST::ico::schedule_next_contributor_payout( _db, a.symbol ) )
      if ( !SST::ico::schedule_founder_payout( _db, a.symbol ) )
         util::SST::ico::remove_ico_objects( _db, a.symbol );
}

void SST_founder_payout_evaluator::do_apply( const SST_founder_payout_action& a )
{
   using namespace freezone::chain::util;
   const auto& token = _db.get< SST_token_object, by_symbol >( a.symbol );

   share_type additional_token_supply = 0;

   for ( auto& account_payout : a.account_payouts )
      additional_token_supply += SST::ico::payout( _db, token.liquid_symbol, _db.get_account( account_payout.first ), account_payout.second );

   _db.modify( token, [&]( SST_token_object& o )
   {
      o.market_maker.token_balance = asset( a.market_maker_tokens, a.symbol );
      o.market_maker.freezone_balance = asset( a.market_maker_freezone, freezone_SYMBOL );
      o.reward_balance = asset( a.reward_balance, a.symbol );
   } );

   additional_token_supply += a.market_maker_tokens;
   additional_token_supply += a.reward_balance;

   if ( additional_token_supply > 0 )
      _db.adjust_supply( asset( additional_token_supply, a.symbol ) );

   _db.modify( token, []( SST_token_object& obj )
   {
      obj.total_vesting_fund_ballast   = ( obj.current_supply * SST_BALLAST_SUPPLY_PERCENT ) / freezone_100_PERCENT;
      obj.total_vesting_shares_ballast = obj.total_vesting_fund_ballast * SST_INITIAL_VESTING_PER_UNIT;
   } );

   util::SST::ico::remove_ico_objects( _db, a.symbol );
}


} } //freezone::chain
