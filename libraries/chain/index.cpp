
#include <freezone/chain/freezone_object_types.hpp>

#include <freezone/chain/index.hpp>

#include <freezone/chain/block_summary_object.hpp>
#include <freezone/chain/history_object.hpp>
#include <freezone/chain/pending_required_action_object.hpp>
#include <freezone/chain/pending_optional_action_object.hpp>
#include <freezone/chain/SST_objects.hpp>
#include <freezone/chain/freezone_objects.hpp>
#include <freezone/chain/sps_objects.hpp>
#include <freezone/chain/transaction_object.hpp>
#include <freezone/chain/witness_schedule.hpp>

namespace freezone { namespace chain {

void initialize_core_indexes( database& db )
{
   freezone_ADD_CORE_INDEX(db, dynamic_global_property_index);
   freezone_ADD_CORE_INDEX(db, account_index);
   freezone_ADD_CORE_INDEX(db, account_metadata_index);
   freezone_ADD_CORE_INDEX(db, account_authority_index);
   freezone_ADD_CORE_INDEX(db, witness_index);
   freezone_ADD_CORE_INDEX(db, transaction_index);
   freezone_ADD_CORE_INDEX(db, block_summary_index);
   freezone_ADD_CORE_INDEX(db, witness_schedule_index);
   freezone_ADD_CORE_INDEX(db, comment_index);
   freezone_ADD_CORE_INDEX(db, comment_content_index);
   freezone_ADD_CORE_INDEX(db, comment_vote_index);
   freezone_ADD_CORE_INDEX(db, witness_vote_index);
   freezone_ADD_CORE_INDEX(db, limit_order_index);
   freezone_ADD_CORE_INDEX(db, feed_history_index);
   freezone_ADD_CORE_INDEX(db, convert_request_index);
   freezone_ADD_CORE_INDEX(db, liquidity_reward_balance_index);
   freezone_ADD_CORE_INDEX(db, operation_index);
   freezone_ADD_CORE_INDEX(db, account_history_index);
   freezone_ADD_CORE_INDEX(db, hardfork_property_index);
   freezone_ADD_CORE_INDEX(db, withdraw_vesting_route_index);
   freezone_ADD_CORE_INDEX(db, owner_authority_history_index);
   freezone_ADD_CORE_INDEX(db, account_recovery_request_index);
   freezone_ADD_CORE_INDEX(db, change_recovery_account_request_index);
   freezone_ADD_CORE_INDEX(db, escrow_index);
   freezone_ADD_CORE_INDEX(db, savings_withdraw_index);
   freezone_ADD_CORE_INDEX(db, decline_voting_rights_request_index);
   freezone_ADD_CORE_INDEX(db, reward_fund_index);
   freezone_ADD_CORE_INDEX(db, vesting_delegation_index);
   freezone_ADD_CORE_INDEX(db, vesting_delegation_expiration_index);
   freezone_ADD_CORE_INDEX(db, pending_required_action_index);
   freezone_ADD_CORE_INDEX(db, pending_optional_action_index);
   freezone_ADD_CORE_INDEX(db, SST_token_index);
   freezone_ADD_CORE_INDEX(db, account_regular_balance_index);
   freezone_ADD_CORE_INDEX(db, account_rewards_balance_index);
   freezone_ADD_CORE_INDEX(db, nai_pool_index);
   freezone_ADD_CORE_INDEX(db, SST_token_emissions_index);
   freezone_ADD_CORE_INDEX(db, SST_contribution_index);
   freezone_ADD_CORE_INDEX(db, SST_ico_index);
   freezone_ADD_CORE_INDEX(db, SST_ico_tier_index);
   freezone_ADD_CORE_INDEX(db, comment_SST_beneficiaries_index);
   freezone_ADD_CORE_INDEX(db, proposal_index);
   freezone_ADD_CORE_INDEX(db, proposal_vote_index);
}

index_info::index_info() {}
index_info::~index_info() {}

abstract_object::abstract_object() {}
abstract_object::~abstract_object() {}

} }
