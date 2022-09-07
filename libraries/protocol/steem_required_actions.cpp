#include <freezone/protocol/validation.hpp>
#include <freezone/protocol/freezone_required_actions.hpp>

namespace freezone { namespace protocol {

#ifdef IS_TEST_NET
void example_required_action::validate()const
{
   validate_account_name( account );
}

bool operator==( const example_required_action& lhs, const example_required_action& rhs )
{
   return lhs.account == rhs.account;
}
#endif

void SST_ico_launch_action::validate() const
{
   validate_account_name( control_account );
   validate_SST_symbol( symbol );
}

bool operator==( const SST_ico_launch_action& lhs, const SST_ico_launch_action& rhs )
{
   return
      lhs.control_account == rhs.control_account &&
      lhs.symbol == rhs.symbol;
}

void SST_ico_evaluation_action::validate() const
{
   validate_account_name( control_account );
   validate_SST_symbol( symbol );
}

bool operator==( const SST_ico_evaluation_action& lhs, const SST_ico_evaluation_action& rhs )
{
   return
      lhs.control_account == rhs.control_account &&
      lhs.symbol == rhs.symbol;
}

void SST_token_launch_action::validate() const
{
   validate_account_name( control_account );
   validate_SST_symbol( symbol );
}

bool operator==( const SST_token_launch_action& lhs, const SST_token_launch_action& rhs )
{
   return
      lhs.control_account == rhs.control_account &&
      lhs.symbol == rhs.symbol;
}

void SST_refund_action::validate() const
{
   validate_account_name( contributor );
   validate_SST_symbol( symbol );
}

bool operator==( const SST_refund_action& lhs, const SST_refund_action& rhs )
{
   return
      lhs.symbol == rhs.symbol &&
      lhs.contributor == rhs.contributor &&
      lhs.contribution_id == rhs.contribution_id &&
      lhs.refund == rhs.refund;
}

void SST_contributor_payout_action::validate() const
{
   validate_account_name( contributor );
   validate_SST_symbol( symbol );
}

bool operator==( const contribution_payout& rhs, const contribution_payout& lhs )
{
   return
      rhs.payout == lhs.payout &&
      rhs.to_vesting == lhs.to_vesting;
}

bool operator==( const SST_contributor_payout_action& lhs, const SST_contributor_payout_action& rhs )
{
   return
      lhs.symbol == rhs.symbol &&
      lhs.contributor == rhs.contributor &&
      lhs.contribution_id == rhs.contribution_id &&
      lhs.contribution == rhs.contribution &&
      lhs.payouts == rhs.payouts;
}

void SST_founder_payout_action::validate() const
{
   validate_SST_symbol( symbol );

   for ( auto& payout : account_payouts )
      validate_account_name( payout.first );
}

bool operator==( const SST_founder_payout_action& lhs, const SST_founder_payout_action& rhs )
{
   return
      lhs.symbol == rhs.symbol &&
      lhs.account_payouts == rhs.account_payouts &&
      lhs.market_maker_freezone == rhs.market_maker_freezone &&
      lhs.market_maker_tokens == rhs.market_maker_tokens &&
      lhs.reward_balance == rhs.reward_balance;
}

} } //freezone::protocol
