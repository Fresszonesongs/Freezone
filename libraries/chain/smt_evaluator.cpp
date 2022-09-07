#include <freezone/chain/freezone_fwd.hpp>

#include <freezone/chain/freezone_evaluator.hpp>
#include <freezone/chain/database.hpp>
#include <freezone/chain/freezone_objects.hpp>
#include <freezone/chain/SST_objects.hpp>
#include <freezone/chain/util/reward.hpp>
#include <freezone/chain/util/SST_token.hpp>

#include <freezone/protocol/SST_operations.hpp>
#include <freezone/protocol/SST_util.hpp>

namespace freezone { namespace chain {

namespace {

/// Return SST token object controlled by this account identified by its symbol. Throws assert exception when not found!
inline const SST_token_object& get_controlled_SST( const database& db, const account_name_type& control_account, const asset_symbol_type& SST_symbol )
{
   const SST_token_object* SST = db.find< SST_token_object, by_symbol >( SST_symbol );
   // The SST is supposed to be found.
   FC_ASSERT( SST != nullptr, "SST numerical asset identifier ${SST} not found", ("SST", SST_symbol.to_nai()) );
   // Check against unotherized account trying to access (and alter) SST. Unotherized means "other than the one that created the SST".
   FC_ASSERT( SST->control_account == control_account, "The account ${account} does NOT control the SST ${SST}",
      ("account", control_account)("SST", SST_symbol.to_nai()) );

   return *SST;
}

}

namespace {

class SST_setup_parameters_visitor : public fc::visitor<bool>
{
public:
   SST_setup_parameters_visitor( SST_token_object& SST_token ) : _SST_token( SST_token ) {}

   bool operator () ( const SST_param_allow_voting& allow_voting )
   {
      _SST_token.allow_voting = allow_voting.value;
      return true;
   }

private:
   SST_token_object& _SST_token;
};

const SST_token_object& common_pre_setup_evaluation(
   const database& _db, const asset_symbol_type& symbol, const account_name_type& control_account )
{
   const SST_token_object& SST = get_controlled_SST( _db, control_account, symbol );

   // Check whether it's not too late to setup emissions operation.
   FC_ASSERT( SST.phase < SST_phase::setup_completed, "SST pre-setup operation no longer allowed after setup phase is over" );

   return SST;
}

} // namespace

void SST_create_evaluator::do_apply( const SST_create_operation& o )
{
   FC_ASSERT( _db.has_hardfork( freezone_SST_HARDFORK ), "SST functionality not enabled until hardfork ${hf}", ("hf", freezone_SST_HARDFORK) );
   const dynamic_global_property_object& dgpo = _db.get_dynamic_global_properties();

   auto token_ptr = util::SST::find_token( _db, o.symbol, true );

   if ( o.SST_creation_fee.amount > 0 ) // Creation case
   {
      FC_ASSERT( token_ptr == nullptr, "SST ${nai} has already been created.", ("nai", o.symbol.to_nai() ) );
      FC_ASSERT( _db.get< nai_pool_object >().contains( o.symbol ), "Cannot create an SST that didn't come from the NAI pool." );

      asset creation_fee;

      if( o.SST_creation_fee.symbol == dgpo.SST_creation_fee.symbol )
      {
         creation_fee = o.SST_creation_fee;
      }
      else
      {
         const auto& fhistory = _db.get_feed_history();
         FC_ASSERT( !fhistory.current_median_history.is_null(), "Cannot pay the fee using different asset symbol because there is no price feed." );

         if( dgpo.SST_creation_fee.symbol == freezone_SYMBOL )
            creation_fee = _db.to_freezone( o.SST_creation_fee );
         else
            creation_fee = _db.to_sbd( o.SST_creation_fee );
      }

      FC_ASSERT( creation_fee == dgpo.SST_creation_fee,
         "Fee of ${ef} does not match the creation fee of ${sf}", ("ef", creation_fee)("sf", dgpo.SST_creation_fee) );

      _db.adjust_balance( o.control_account , -o.SST_creation_fee );
      _db.adjust_balance( freezone_NULL_ACCOUNT,  o.SST_creation_fee );
   }
   else // Reset case
   {
      FC_ASSERT( token_ptr != nullptr, "Cannot reset a non-existent SST. Did you forget to specify the creation fee?" );
      FC_ASSERT( token_ptr->control_account == o.control_account, "You do not control this SST. Control Account: ${a}", ("a", token_ptr->control_account) );
      FC_ASSERT( token_ptr->phase == SST_phase::setup, "SST cannot be reset if setup is completed. Phase: ${p}", ("p", token_ptr->phase) );
      FC_ASSERT( !util::SST::last_emission_time( _db, token_ptr->liquid_symbol ), "Cannot reset an SST that has existing token emissions." );
      FC_ASSERT( util::SST::ico::ico_tier_size(_db, token_ptr->liquid_symbol ) == 0, "Cannot reset an SST that has existing ICO tiers." );

      _db.remove( *token_ptr );
   }

   // Create SST object common to both liquid and vesting variants of SST.
   _db.create< SST_token_object >( [&]( SST_token_object& token )
   {
      token.liquid_symbol = o.symbol;
      token.control_account = o.control_account;
      token.desired_ticker = o.desired_ticker;
      token.market_maker.token_balance = asset( 0, token.liquid_symbol );
      token.reward_balance = asset( 0, token.liquid_symbol );
   });

   remove_from_nai_pool( _db, o.symbol );

   if ( !_db.is_pending_tx() )
      replenish_nai_pool( _db );
}

static void verify_accounts( database& db, const flat_map< unit_target_type, uint16_t >& units )
{
   for ( auto& unit : units )
   {
      if ( !protocol::SST::unit_target::is_account_name_type( unit.first ) )
         continue;

      auto account_name = protocol::SST::unit_target::get_unit_target_account( unit.first );
      const auto* account = db.find_account( account_name );
      FC_ASSERT( account != nullptr, "The provided account unit target ${target} does not exist.", ("target", unit.first) );
   }
}

struct SST_generation_policy_verifier
{
   database& _db;

   SST_generation_policy_verifier( database& db ): _db( db ){}

   typedef void result_type;

   void operator()( const SST_capped_generation_policy& capped_generation_policy ) const
   {
      verify_accounts( _db, capped_generation_policy.generation_unit.freezone_unit );
      verify_accounts( _db, capped_generation_policy.generation_unit.token_unit );
   }
};

template< class T >
struct SST_generation_policy_visitor
{
   T& _obj;

   SST_generation_policy_visitor( T& o ): _obj( o ) {}

   typedef void result_type;

   void operator()( const SST_capped_generation_policy& capped_generation_policy ) const
   {
      _obj = capped_generation_policy;
   }
};

void SST_setup_evaluator::do_apply( const SST_setup_operation& o )
{
   FC_ASSERT( _db.has_hardfork( freezone_SST_HARDFORK ), "SST functionality not enabled until hardfork ${hf}", ("hf", freezone_SST_HARDFORK) );

   const SST_token_object& _token = common_pre_setup_evaluation( _db, o.symbol, o.control_account );
   share_type hard_cap;

   if ( o.freezone_satoshi_min > 0 )
   {
      auto possible_hard_cap = util::SST::ico::get_ico_freezone_hard_cap( _db, o.symbol );

      FC_ASSERT( possible_hard_cap.valid(),
         "An SST with a freezone Satoshi Minimum of ${s} cannot succeed without an ICO tier.", ("s", o.freezone_satoshi_min) );

      hard_cap = *possible_hard_cap;

      FC_ASSERT( o.freezone_satoshi_min <= hard_cap,
         "The freezone Satoshi Minimum must be less than the hard cap. freezone Satoshi Minimum: ${s}, Hard Cap: ${c}",
         ("s", o.freezone_satoshi_min)("c", hard_cap) );
   }

   auto ico_tiers = _db.get_index< SST_ico_tier_index, by_symbol_freezone_satoshi_cap >().equal_range( _token.liquid_symbol );
   share_type prev_tier_cap = 0;
   share_type total_tokens = 0;

   for( ; ico_tiers.first != ico_tiers.second; ++ico_tiers.first )
   {
      fc::uint128_t max_token_units = ( hard_cap / ico_tiers.first->generation_unit.freezone_unit_sum() ).value * o.min_unit_ratio;
      FC_ASSERT( max_token_units.hi == 0 && max_token_units.lo <= uint64_t( std::numeric_limits<int64_t>::max() ),
         "Overflow detected in ICO tier '${t}'", ("tier", *ico_tiers.first) );

      // This is done with share_types, which are safe< int64_t >. Overflow is detected but does not provide
      // a useful error message. Do the checks manually to provide actionable information.
      fc::uint128_t new_tokens = ( ( ico_tiers.first->freezone_satoshi_cap - prev_tier_cap ).value / ico_tiers.first->generation_unit.freezone_unit_sum() );

      new_tokens *= o.min_unit_ratio;
      FC_ASSERT( new_tokens.hi == 0 && new_tokens.lo <= uint64_t( std::numeric_limits<int64_t>::max() ),
         "Overflow detected in ICO tier '${t}'", ("tier", *ico_tiers.first) );

      new_tokens *= ico_tiers.first->generation_unit.token_unit_sum();
      FC_ASSERT( new_tokens.hi == 0 && new_tokens.lo <= uint64_t( std::numeric_limits<int64_t>::max() ),
         "Overflow detected in ICO tier '${t}'", ("tier", *ico_tiers.first) );

      total_tokens += new_tokens.to_int64();
      prev_tier_cap = ico_tiers.first->freezone_satoshi_cap;
   }

   FC_ASSERT( total_tokens < freezone_MAX_SHARE_SUPPLY,
      "Max token supply for ${n} can exceed ${m}. Calculated: ${c}",
      ("n", _token.liquid_symbol)
      ("m", freezone_MAX_SHARE_SUPPLY)
      ("c", total_tokens) );

   _db.modify( _token, [&]( SST_token_object& token )
   {
      token.phase = SST_phase::setup_completed;
      token.max_supply = o.max_supply;
   } );

   _db.create< SST_ico_object >( [&] ( SST_ico_object& token_ico_obj )
   {
      token_ico_obj.symbol = _token.liquid_symbol;
      token_ico_obj.contribution_begin_time = o.contribution_begin_time;
      token_ico_obj.contribution_end_time = o.contribution_end_time;
      token_ico_obj.launch_time = o.launch_time;
      token_ico_obj.freezone_satoshi_min = o.freezone_satoshi_min;
      token_ico_obj.min_unit_ratio = o.min_unit_ratio;
      token_ico_obj.max_unit_ratio = o.max_unit_ratio;
   } );

   SST_ico_launch_action ico_launch_action;
   ico_launch_action.control_account = _token.control_account;
   ico_launch_action.symbol = _token.liquid_symbol;
   _db.push_required_action( ico_launch_action, o.contribution_begin_time );
}

void SST_setup_ico_tier_evaluator::do_apply( const SST_setup_ico_tier_operation& o )
{
   FC_ASSERT( _db.has_hardfork( freezone_SST_HARDFORK ), "SST functionality not enabled until hardfork ${hf}", ("hf", freezone_SST_HARDFORK) );

   const SST_token_object& token = common_pre_setup_evaluation( _db, o.symbol, o.control_account );

   SST_capped_generation_policy generation_policy;
   SST_generation_policy_visitor< SST_capped_generation_policy > visitor( generation_policy );
   o.generation_policy.visit( visitor );

   if ( o.remove )
   {
      auto key = boost::make_tuple( token.liquid_symbol, o.freezone_satoshi_cap );
      const auto* ito = _db.find< SST_ico_tier_object, by_symbol_freezone_satoshi_cap >( key );

      FC_ASSERT( ito != nullptr,
         "The specified ICO tier does not exist. Symbol: ${s}, freezone Satoshi Cap: ${c}",
         ("s", token.liquid_symbol)("c", o.freezone_satoshi_cap)
      );

      _db.remove( *ito );
   }
   else
   {
      auto num_ico_tiers = util::SST::ico::ico_tier_size( _db, o.symbol );
      FC_ASSERT( num_ico_tiers < SST_MAX_ICO_TIERS,
         "There can be a maximum of ${n} ICO tiers. Current: ${c}", ("n", SST_MAX_ICO_TIERS)("c", num_ico_tiers) );

      SST_generation_policy_verifier generation_policy_verifier( _db );
      o.generation_policy.visit( generation_policy_verifier );

      _db.create< SST_ico_tier_object >( [&]( SST_ico_tier_object& ito )
      {
         ito.symbol                   = token.liquid_symbol;
         ito.freezone_satoshi_cap        = o.freezone_satoshi_cap;
         ito.generation_unit          = generation_policy.generation_unit;
      });
   }
}

void SST_setup_emissions_evaluator::do_apply( const SST_setup_emissions_operation& o )
{
   FC_ASSERT( _db.has_hardfork( freezone_SST_HARDFORK ), "SST functionality not enabled until hardfork ${hf}", ("hf", freezone_SST_HARDFORK) );

   const SST_token_object& SST = common_pre_setup_evaluation( _db, o.symbol, o.control_account );

   if ( o.remove )
   {
      auto last_emission = util::SST::last_emission( _db, o.symbol );
      FC_ASSERT( last_emission != nullptr, "Could not find token emission for the given SST: ${SST}", ("SST", o.symbol) );

      FC_ASSERT(
         last_emission->symbol == o.symbol &&
         last_emission->schedule_time == o.schedule_time,
         "SST emissions must be removed from latest to earliest, last emission: ${le}. Current: ${c}", ("le", *last_emission)("c", o)
      );

      _db.remove( *last_emission );
   }
   else
   {
      FC_ASSERT( o.schedule_time > _db.head_block_time(), "Emissions schedule time must be in the future" );

      auto end_time = util::SST::last_emission_time( _db, SST.liquid_symbol );

      if ( end_time.valid() )
      {
         FC_ASSERT( ( time_point_sec::maximum() - *end_time ) > fc::seconds( SST_EMISSION_MIN_INTERVAL_SECONDS ),
            "Cannot add token emission when the prior emission is indefinite." );
         FC_ASSERT( o.schedule_time >= *end_time + SST_EMISSION_MIN_INTERVAL_SECONDS,
            "New token emissions must begin no earlier than ${s} seconds after the last emission. Last emission time: ${end}",
            ("s", SST_EMISSION_MIN_INTERVAL_SECONDS)("end", *end_time) );
      }

      for ( const auto& e : o.emissions_unit.token_unit )
      {
         if ( SST::unit_target::is_account_name_type( e.first ) )
         {
            std::string name = SST::unit_target::get_unit_target_account( e.first );
            auto acc = _db.find< account_object, by_name >( name );
            FC_ASSERT( acc != nullptr, "Invalid emission destination, account ${a} must exist", ("a", name) );
         }
      }

      _db.create< SST_token_emissions_object >( [&]( SST_token_emissions_object& eo )
      {
         eo.symbol = SST.liquid_symbol;
         eo.schedule_time = o.schedule_time;
         eo.emissions_unit = o.emissions_unit;
         eo.interval_seconds = o.interval_seconds;
         eo.emission_count = o.emission_count;
         eo.lep_time = o.lep_time;
         eo.rep_time = o.rep_time;
         eo.lep_abs_amount = o.lep_abs_amount;
         eo.rep_abs_amount = o.rep_abs_amount;
         eo.lep_rel_amount_numerator = o.lep_rel_amount_numerator;
         eo.rep_rel_amount_numerator = o.rep_rel_amount_numerator;
         eo.rel_amount_denom_bits = o.rel_amount_denom_bits;
         eo.floor_emissions = o.floor_emissions;
      });
   }
}

void SST_set_setup_parameters_evaluator::do_apply( const SST_set_setup_parameters_operation& o )
{
   FC_ASSERT( _db.has_hardfork( freezone_SST_HARDFORK ), "SST functionality not enabled until hardfork ${hf}", ("hf", freezone_SST_HARDFORK) );

   const SST_token_object& SST_token = common_pre_setup_evaluation( _db, o.symbol, o.control_account );

   _db.modify( SST_token, [&]( SST_token_object& token )
   {
      SST_setup_parameters_visitor visitor( token );

      for ( auto& param : o.setup_parameters )
         param.visit( visitor );
   });
}

struct SST_set_runtime_parameters_evaluator_visitor
{
   SST_token_object& _token;

   SST_set_runtime_parameters_evaluator_visitor( SST_token_object& token ) : _token( token ) {}

   typedef void result_type;

   void operator()( const SST_param_windows_v1& param_windows )const
   {
      _token.cashout_window_seconds = param_windows.cashout_window_seconds;
      _token.reverse_auction_window_seconds = param_windows.reverse_auction_window_seconds;
   }

   void operator()( const SST_param_vote_regeneration_period_seconds_v1& vote_regeneration )const
   {
      _token.vote_regeneration_period_seconds = vote_regeneration.vote_regeneration_period_seconds;
      _token.votes_per_regeneration_period = vote_regeneration.votes_per_regeneration_period;
   }

   void operator()( const SST_param_rewards_v1& param_rewards )const
   {
      _token.content_constant = param_rewards.content_constant;
      _token.percent_curation_rewards = param_rewards.percent_curation_rewards;
      _token.author_reward_curve = param_rewards.author_reward_curve;
      _token.curation_reward_curve = param_rewards.curation_reward_curve;
   }

   void operator()( const SST_param_allow_downvotes& param_allow_downvotes )const
   {
      _token.allow_downvotes = param_allow_downvotes.value;
   }
};

void SST_set_runtime_parameters_evaluator::do_apply( const SST_set_runtime_parameters_operation& o )
{
   FC_ASSERT( _db.has_hardfork( freezone_SST_HARDFORK ), "SST functionality not enabled until hardfork ${hf}", ("hf", freezone_SST_HARDFORK) );

   const SST_token_object& token = common_pre_setup_evaluation(_db, o.symbol, o.control_account);

   _db.modify( token, [&]( SST_token_object& t )
   {
      SST_set_runtime_parameters_evaluator_visitor visitor( t );

      for( auto& param: o.runtime_parameters )
         param.visit( visitor );
   });
}

void SST_contribute_evaluator::do_apply( const SST_contribute_operation& o )
{
   try
   {
      FC_ASSERT( _db.has_hardfork( freezone_SST_HARDFORK ), "SST functionality not enabled until hardfork ${hf}", ("hf", freezone_SST_HARDFORK) );

      const SST_token_object* token = util::SST::find_token( _db, o.symbol );
      FC_ASSERT( token != nullptr, "Cannot contribute to an unknown SST" );
      FC_ASSERT( token->phase >= SST_phase::ico, "SST has not begun accepting contributions" );
      FC_ASSERT( token->phase < SST_phase::ico_completed, "SST is no longer accepting contributions" );

      auto possible_hard_cap = util::SST::ico::get_ico_freezone_hard_cap( _db, o.symbol );
      FC_ASSERT( possible_hard_cap.valid(), "The specified token does not feature an ICO" );
      share_type hard_cap = *possible_hard_cap;

      const SST_ico_object* token_ico = _db.find< SST_ico_object, by_symbol >( token->liquid_symbol );
      FC_ASSERT( token_ico != nullptr, "Unable to find ICO data for symbol: ${sym}", ("sym", token->liquid_symbol) );
      FC_ASSERT( token_ico->contributed.amount < hard_cap, "SST ICO has reached its hard cap and no longer accepts contributions" );
      FC_ASSERT( token_ico->contributed.amount + o.contribution.amount <= hard_cap,
         "The proposed contribution would exceed the ICO hard cap, maximum possible contribution: ${c}",
         ("c", asset( hard_cap - token_ico->contributed.amount, freezone_SYMBOL )) );

      auto key = boost::tuple< asset_symbol_type, account_name_type, uint32_t >( o.contribution.symbol, o.contributor, o.contribution_id );
      auto contrib_ptr = _db.find< SST_contribution_object, by_symbol_contributor >( key );
      FC_ASSERT( contrib_ptr == nullptr, "The provided contribution ID must be unique. Current: ${id}", ("id", o.contribution_id) );

      _db.adjust_balance( o.contributor, -o.contribution );

      _db.create< SST_contribution_object >( [&] ( SST_contribution_object& obj )
      {
         obj.contributor = o.contributor;
         obj.symbol = o.symbol;
         obj.contribution_id = o.contribution_id;
         obj.contribution = o.contribution;
      } );

      _db.modify( *token_ico, [&]( SST_ico_object& ico )
      {
         ico.contributed += o.contribution;
      } );
   }
   FC_CAPTURE_AND_RETHROW( (o) )
}

} }
