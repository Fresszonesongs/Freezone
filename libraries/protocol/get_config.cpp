#include <freezone/protocol/get_config.hpp>
#include <freezone/protocol/config.hpp>
#include <freezone/protocol/asset.hpp>
#include <freezone/protocol/types.hpp>
#include <freezone/protocol/version.hpp>

namespace freezone { namespace protocol {

fc::variant_object get_config()
{
   fc::mutable_variant_object result;

#ifdef IS_TEST_NET
   result[ "IS_TEST_NET" ] = true;
   result["TESTNET_BLOCK_LIMIT"] = TESTNET_BLOCK_LIMIT;
#else
   result[ "IS_TEST_NET" ] = false;
#endif

   result["SST_MAX_VOTABLE_ASSETS"] = SST_MAX_VOTABLE_ASSETS;
   result["SST_VESTING_WITHDRAW_INTERVAL_SECONDS"] = SST_VESTING_WITHDRAW_INTERVAL_SECONDS;
   result["SST_UPVOTE_LOCKOUT"] = SST_UPVOTE_LOCKOUT;
   result["SST_EMISSION_MIN_INTERVAL_SECONDS"] = SST_EMISSION_MIN_INTERVAL_SECONDS;
   result["SST_EMIT_INDEFINITELY"] = SST_EMIT_INDEFINITELY;
   result["SST_MAX_NOMINAL_VOTES_PER_DAY"] = SST_MAX_NOMINAL_VOTES_PER_DAY;
   result["SST_MAX_VOTES_PER_REGENERATION"] = SST_MAX_VOTES_PER_REGENERATION;
   result["SST_DEFAULT_VOTES_PER_REGEN_PERIOD"] = SST_DEFAULT_VOTES_PER_REGEN_PERIOD;
   result["SST_DEFAULT_PERCENT_CURATION_REWARDS"] = SST_DEFAULT_PERCENT_CURATION_REWARDS;
   result["SST_INITIAL_VESTING_PER_UNIT"] = SST_INITIAL_VESTING_PER_UNIT;
   result["SST_BALLAST_SUPPLY_PERCENT"] = SST_BALLAST_SUPPLY_PERCENT;
   result["SST_MAX_ICO_TIERS"] = SST_MAX_ICO_TIERS;

   result["SBD_SYMBOL"] = SBD_SYMBOL;
   result["freezone_INITIAL_VOTE_POWER_RATE"] = freezone_INITIAL_VOTE_POWER_RATE;
   result["freezone_REDUCED_VOTE_POWER_RATE"] = freezone_REDUCED_VOTE_POWER_RATE;
   result["freezone_100_PERCENT"] = freezone_100_PERCENT;
   result["freezone_1_PERCENT"] = freezone_1_PERCENT;
   result["freezone_ACCOUNT_RECOVERY_REQUEST_EXPIRATION_PERIOD"] = freezone_ACCOUNT_RECOVERY_REQUEST_EXPIRATION_PERIOD;
   result["freezone_ACTIVE_CHALLENGE_COOLDOWN"] = freezone_ACTIVE_CHALLENGE_COOLDOWN;
   result["freezone_ACTIVE_CHALLENGE_FEE"] = freezone_ACTIVE_CHALLENGE_FEE;
   result["freezone_ADDRESS_PREFIX"] = freezone_ADDRESS_PREFIX;
   result["freezone_APR_PERCENT_MULTIPLY_PER_BLOCK"] = freezone_APR_PERCENT_MULTIPLY_PER_BLOCK;
   result["freezone_APR_PERCENT_MULTIPLY_PER_HOUR"] = freezone_APR_PERCENT_MULTIPLY_PER_HOUR;
   result["freezone_APR_PERCENT_MULTIPLY_PER_ROUND"] = freezone_APR_PERCENT_MULTIPLY_PER_ROUND;
   result["freezone_APR_PERCENT_SHIFT_PER_BLOCK"] = freezone_APR_PERCENT_SHIFT_PER_BLOCK;
   result["freezone_APR_PERCENT_SHIFT_PER_HOUR"] = freezone_APR_PERCENT_SHIFT_PER_HOUR;
   result["freezone_APR_PERCENT_SHIFT_PER_ROUND"] = freezone_APR_PERCENT_SHIFT_PER_ROUND;
   result["freezone_BANDWIDTH_AVERAGE_WINDOW_SECONDS"] = freezone_BANDWIDTH_AVERAGE_WINDOW_SECONDS;
   result["freezone_BANDWIDTH_PRECISION"] = freezone_BANDWIDTH_PRECISION;
   result["freezone_BENEFICIARY_LIMIT"] = freezone_BENEFICIARY_LIMIT;
   result["freezone_BLOCKCHAIN_PRECISION"] = freezone_BLOCKCHAIN_PRECISION;
   result["freezone_BLOCKCHAIN_PRECISION_DIGITS"] = freezone_BLOCKCHAIN_PRECISION_DIGITS;
   result["freezone_BLOCKCHAIN_HARDFORK_VERSION"] = freezone_BLOCKCHAIN_HARDFORK_VERSION;
   result["freezone_BLOCKCHAIN_VERSION"] = freezone_BLOCKCHAIN_VERSION;
   result["freezone_BLOCK_INTERVAL"] = freezone_BLOCK_INTERVAL;
   result["freezone_BLOCKS_PER_DAY"] = freezone_BLOCKS_PER_DAY;
   result["freezone_BLOCKS_PER_HOUR"] = freezone_BLOCKS_PER_HOUR;
   result["freezone_BLOCKS_PER_YEAR"] = freezone_BLOCKS_PER_YEAR;
   result["freezone_CASHOUT_WINDOW_SECONDS"] = freezone_CASHOUT_WINDOW_SECONDS;
   result["freezone_CASHOUT_WINDOW_SECONDS_PRE_HF12"] = freezone_CASHOUT_WINDOW_SECONDS_PRE_HF12;
   result["freezone_CASHOUT_WINDOW_SECONDS_PRE_HF17"] = freezone_CASHOUT_WINDOW_SECONDS_PRE_HF17;
   result["freezone_CHAIN_ID"] = freezone_CHAIN_ID;
   result["freezone_COMMENT_REWARD_FUND_NAME"] = freezone_COMMENT_REWARD_FUND_NAME;
   result["freezone_COMMENT_TITLE_LIMIT"] = freezone_COMMENT_TITLE_LIMIT;
   result["freezone_CONTENT_APR_PERCENT"] = freezone_CONTENT_APR_PERCENT;
   result["freezone_CONTENT_CONSTANT_HF0"] = freezone_CONTENT_CONSTANT_HF0;
   result["freezone_CONTENT_CONSTANT_HF21"] = freezone_CONTENT_CONSTANT_HF21;
   result["freezone_CONTENT_REWARD_PERCENT_HF16"] = freezone_CONTENT_REWARD_PERCENT_HF16;
   result["freezone_CONTENT_REWARD_PERCENT_HF21"] = freezone_CONTENT_REWARD_PERCENT_HF21;
   result["freezone_CONVERSION_DELAY"] = freezone_CONVERSION_DELAY;
   result["freezone_CONVERSION_DELAY_PRE_HF_16"] = freezone_CONVERSION_DELAY_PRE_HF_16;
   result["freezone_CREATE_ACCOUNT_DELEGATION_RATIO"] = freezone_CREATE_ACCOUNT_DELEGATION_RATIO;
   result["freezone_CREATE_ACCOUNT_DELEGATION_TIME"] = freezone_CREATE_ACCOUNT_DELEGATION_TIME;
   result["freezone_CREATE_ACCOUNT_WITH_freezone_MODIFIER"] = freezone_CREATE_ACCOUNT_WITH_freezone_MODIFIER;
   result["freezone_CURATE_APR_PERCENT"] = freezone_CURATE_APR_PERCENT;
   result["freezone_CUSTOM_OP_DATA_MAX_LENGTH"] = freezone_CUSTOM_OP_DATA_MAX_LENGTH;
   result["freezone_CUSTOM_OP_ID_MAX_LENGTH"] = freezone_CUSTOM_OP_ID_MAX_LENGTH;
   result["freezone_DEFAULT_SBD_INTEREST_RATE"] = freezone_DEFAULT_SBD_INTEREST_RATE;
   result["freezone_DOWNVOTE_POOL_PERCENT_HF21"] = freezone_DOWNVOTE_POOL_PERCENT_HF21;
   result["freezone_EQUIHASH_K"] = freezone_EQUIHASH_K;
   result["freezone_EQUIHASH_N"] = freezone_EQUIHASH_N;
   result["freezone_FEED_HISTORY_WINDOW"] = freezone_FEED_HISTORY_WINDOW;
   result["freezone_FEED_HISTORY_WINDOW_PRE_HF_16"] = freezone_FEED_HISTORY_WINDOW_PRE_HF_16;
   result["freezone_FEED_INTERVAL_BLOCKS"] = freezone_FEED_INTERVAL_BLOCKS;
   result["freezone_GENESIS_TIME"] = freezone_GENESIS_TIME;
   result["freezone_HARDFORK_REQUIRED_WITNESSES"] = freezone_HARDFORK_REQUIRED_WITNESSES;
   result["freezone_HF21_CONVERGENT_LINEAR_RECENT_CLAIMS"] = freezone_HF21_CONVERGENT_LINEAR_RECENT_CLAIMS;
   result["freezone_INFLATION_NARROWING_PERIOD"] = freezone_INFLATION_NARROWING_PERIOD;
   result["freezone_INFLATION_RATE_START_PERCENT"] = freezone_INFLATION_RATE_START_PERCENT;
   result["freezone_INFLATION_RATE_STOP_PERCENT"] = freezone_INFLATION_RATE_STOP_PERCENT;
   result["freezone_INIT_MINER_NAME"] = freezone_INIT_MINER_NAME;
   result["freezone_INIT_PUBLIC_KEY_STR"] = freezone_INIT_PUBLIC_KEY_STR;
#if 0
   // do not expose private key, period.
   // we need this line present but inactivated so CI check for all constants in config.hpp doesn't complain.
   result["freezone_INIT_PRIVATE_KEY"] = freezone_INIT_PRIVATE_KEY;
#endif
   result["freezone_INIT_SUPPLY"] = freezone_INIT_SUPPLY;
   result["freezone_SBD_INIT_SUPPLY"] = freezone_SBD_INIT_SUPPLY;
   result["freezone_INIT_TIME"] = freezone_INIT_TIME;
   result["freezone_IRREVERSIBLE_THRESHOLD"] = freezone_IRREVERSIBLE_THRESHOLD;
   result["freezone_LIQUIDITY_APR_PERCENT"] = freezone_LIQUIDITY_APR_PERCENT;
   result["freezone_LIQUIDITY_REWARD_BLOCKS"] = freezone_LIQUIDITY_REWARD_BLOCKS;
   result["freezone_LIQUIDITY_REWARD_PERIOD_SEC"] = freezone_LIQUIDITY_REWARD_PERIOD_SEC;
   result["freezone_LIQUIDITY_TIMEOUT_SEC"] = freezone_LIQUIDITY_TIMEOUT_SEC;
   result["freezone_MAX_ACCOUNT_CREATION_FEE"] = freezone_MAX_ACCOUNT_CREATION_FEE;
   result["freezone_MAX_ACCOUNT_NAME_LENGTH"] = freezone_MAX_ACCOUNT_NAME_LENGTH;
   result["freezone_MAX_ACCOUNT_WITNESS_VOTES"] = freezone_MAX_ACCOUNT_WITNESS_VOTES;
   result["freezone_MAX_ASSET_WHITELIST_AUTHORITIES"] = freezone_MAX_ASSET_WHITELIST_AUTHORITIES;
   result["freezone_MAX_AUTHORITY_MEMBERSHIP"] = freezone_MAX_AUTHORITY_MEMBERSHIP;
   result["freezone_MAX_BLOCK_SIZE"] = freezone_MAX_BLOCK_SIZE;
   result["freezone_SOFT_MAX_BLOCK_SIZE"] = freezone_SOFT_MAX_BLOCK_SIZE;
   result["freezone_MAX_CASHOUT_WINDOW_SECONDS"] = freezone_MAX_CASHOUT_WINDOW_SECONDS;
   result["freezone_MAX_COMMENT_DEPTH"] = freezone_MAX_COMMENT_DEPTH;
   result["freezone_MAX_COMMENT_DEPTH_PRE_HF17"] = freezone_MAX_COMMENT_DEPTH_PRE_HF17;
   result["freezone_MAX_FEED_AGE_SECONDS"] = freezone_MAX_FEED_AGE_SECONDS;
   result["freezone_MAX_INSTANCE_ID"] = freezone_MAX_INSTANCE_ID;
   result["freezone_MAX_MEMO_SIZE"] = freezone_MAX_MEMO_SIZE;
   result["freezone_MAX_WITNESSES"] = freezone_MAX_WITNESSES;
   result["freezone_MAX_MINER_WITNESSES_HF0"] = freezone_MAX_MINER_WITNESSES_HF0;
   result["freezone_MAX_MINER_WITNESSES_HF17"] = freezone_MAX_MINER_WITNESSES_HF17;
   result["freezone_MAX_PERMLINK_LENGTH"] = freezone_MAX_PERMLINK_LENGTH;
   result["freezone_MAX_PROXY_RECURSION_DEPTH"] = freezone_MAX_PROXY_RECURSION_DEPTH;
   result["freezone_MAX_RATION_DECAY_RATE"] = freezone_MAX_RATION_DECAY_RATE;
   result["freezone_MAX_RESERVE_RATIO"] = freezone_MAX_RESERVE_RATIO;
   result["freezone_MAX_RUNNER_WITNESSES_HF0"] = freezone_MAX_RUNNER_WITNESSES_HF0;
   result["freezone_MAX_RUNNER_WITNESSES_HF17"] = freezone_MAX_RUNNER_WITNESSES_HF17;
   result["freezone_MAX_SATOSHIS"] = freezone_MAX_SATOSHIS;
   result["freezone_MAX_SHARE_SUPPLY"] = freezone_MAX_SHARE_SUPPLY;
   result["freezone_MAX_SIG_CHECK_DEPTH"] = freezone_MAX_SIG_CHECK_DEPTH;
   result["freezone_MAX_SIG_CHECK_ACCOUNTS"] = freezone_MAX_SIG_CHECK_ACCOUNTS;
   result["freezone_MAX_TIME_UNTIL_EXPIRATION"] = freezone_MAX_TIME_UNTIL_EXPIRATION;
   result["freezone_MAX_TRANSACTION_SIZE"] = freezone_MAX_TRANSACTION_SIZE;
   result["freezone_MAX_UNDO_HISTORY"] = freezone_MAX_UNDO_HISTORY;
   result["freezone_MAX_URL_LENGTH"] = freezone_MAX_URL_LENGTH;
   result["freezone_MAX_VOTE_CHANGES"] = freezone_MAX_VOTE_CHANGES;
   result["freezone_MAX_VOTED_WITNESSES_HF0"] = freezone_MAX_VOTED_WITNESSES_HF0;
   result["freezone_MAX_VOTED_WITNESSES_HF17"] = freezone_MAX_VOTED_WITNESSES_HF17;
   result["freezone_MAX_WITHDRAW_ROUTES"] = freezone_MAX_WITHDRAW_ROUTES;
   result["freezone_MAX_WITNESS_URL_LENGTH"] = freezone_MAX_WITNESS_URL_LENGTH;
   result["freezone_MIN_ACCOUNT_CREATION_FEE"] = freezone_MIN_ACCOUNT_CREATION_FEE;
   result["freezone_MIN_ACCOUNT_NAME_LENGTH"] = freezone_MIN_ACCOUNT_NAME_LENGTH;
   result["freezone_MIN_BLOCK_SIZE_LIMIT"] = freezone_MIN_BLOCK_SIZE_LIMIT;
   result["freezone_MIN_BLOCK_SIZE"] = freezone_MIN_BLOCK_SIZE;
   result["freezone_MIN_CONTENT_REWARD"] = freezone_MIN_CONTENT_REWARD;
   result["freezone_MIN_CURATE_REWARD"] = freezone_MIN_CURATE_REWARD;
   result["freezone_MIN_PERMLINK_LENGTH"] = freezone_MIN_PERMLINK_LENGTH;
   result["freezone_MIN_REPLY_INTERVAL"] = freezone_MIN_REPLY_INTERVAL;
   result["freezone_MIN_REPLY_INTERVAL_HF20"] = freezone_MIN_REPLY_INTERVAL_HF20;
   result["freezone_MIN_ROOT_COMMENT_INTERVAL"] = freezone_MIN_ROOT_COMMENT_INTERVAL;
   result["freezone_MIN_COMMENT_EDIT_INTERVAL"] = freezone_MIN_COMMENT_EDIT_INTERVAL;
   result["freezone_MIN_VOTE_INTERVAL_SEC"] = freezone_MIN_VOTE_INTERVAL_SEC;
   result["freezone_MINER_ACCOUNT"] = freezone_MINER_ACCOUNT;
   result["freezone_MINER_PAY_PERCENT"] = freezone_MINER_PAY_PERCENT;
   result["freezone_MIN_FEEDS"] = freezone_MIN_FEEDS;
   result["freezone_MINING_REWARD"] = freezone_MINING_REWARD;
   result["freezone_MINING_TIME"] = freezone_MINING_TIME;
   result["freezone_MIN_LIQUIDITY_REWARD"] = freezone_MIN_LIQUIDITY_REWARD;
   result["freezone_MIN_LIQUIDITY_REWARD_PERIOD_SEC"] = freezone_MIN_LIQUIDITY_REWARD_PERIOD_SEC;
   result["freezone_MIN_PAYOUT_SBD"] = freezone_MIN_PAYOUT_SBD;
   result["freezone_MIN_POW_REWARD"] = freezone_MIN_POW_REWARD;
   result["freezone_MIN_PRODUCER_REWARD"] = freezone_MIN_PRODUCER_REWARD;
   result["freezone_MIN_TRANSACTION_EXPIRATION_LIMIT"] = freezone_MIN_TRANSACTION_EXPIRATION_LIMIT;
   result["freezone_MIN_TRANSACTION_SIZE_LIMIT"] = freezone_MIN_TRANSACTION_SIZE_LIMIT;
   result["freezone_MIN_UNDO_HISTORY"] = freezone_MIN_UNDO_HISTORY;
   result["freezone_NULL_ACCOUNT"] = freezone_NULL_ACCOUNT;
   result["freezone_NUM_INIT_MINERS"] = freezone_NUM_INIT_MINERS;
   result["freezone_OWNER_AUTH_HISTORY_TRACKING_START_BLOCK_NUM"] = freezone_OWNER_AUTH_HISTORY_TRACKING_START_BLOCK_NUM;
   result["freezone_OWNER_AUTH_RECOVERY_PERIOD"] = freezone_OWNER_AUTH_RECOVERY_PERIOD;
   result["freezone_OWNER_CHALLENGE_COOLDOWN"] = freezone_OWNER_CHALLENGE_COOLDOWN;
   result["freezone_OWNER_CHALLENGE_FEE"] = freezone_OWNER_CHALLENGE_FEE;
   result["freezone_OWNER_UPDATE_LIMIT"] = freezone_OWNER_UPDATE_LIMIT;
   result["freezone_POST_AVERAGE_WINDOW"] = freezone_POST_AVERAGE_WINDOW;
   result["freezone_POST_REWARD_FUND_NAME"] = freezone_POST_REWARD_FUND_NAME;
   result["freezone_POST_WEIGHT_CONSTANT"] = freezone_POST_WEIGHT_CONSTANT;
   result["freezone_POW_APR_PERCENT"] = freezone_POW_APR_PERCENT;
   result["freezone_PRODUCER_APR_PERCENT"] = freezone_PRODUCER_APR_PERCENT;
   result["freezone_PROXY_TO_SELF_ACCOUNT"] = freezone_PROXY_TO_SELF_ACCOUNT;
   result["freezone_SBD_INTEREST_COMPOUND_INTERVAL_SEC"] = freezone_SBD_INTEREST_COMPOUND_INTERVAL_SEC;
   result["freezone_SECONDS_PER_YEAR"] = freezone_SECONDS_PER_YEAR;
   result["freezone_PROPOSAL_FUND_PERCENT_HF0"] = freezone_PROPOSAL_FUND_PERCENT_HF0;
   result["freezone_PROPOSAL_FUND_PERCENT_HF21"] = freezone_PROPOSAL_FUND_PERCENT_HF21;
   result["freezone_RECENT_RSHARES_DECAY_TIME_HF19"] = freezone_RECENT_RSHARES_DECAY_TIME_HF19;
   result["freezone_RECENT_RSHARES_DECAY_TIME_HF17"] = freezone_RECENT_RSHARES_DECAY_TIME_HF17;
   result["freezone_REVERSE_AUCTION_WINDOW_SECONDS_HF6"] = freezone_REVERSE_AUCTION_WINDOW_SECONDS_HF6;
   result["freezone_REVERSE_AUCTION_WINDOW_SECONDS_HF20"] = freezone_REVERSE_AUCTION_WINDOW_SECONDS_HF20;
   result["freezone_REVERSE_AUCTION_WINDOW_SECONDS_HF21"] = freezone_REVERSE_AUCTION_WINDOW_SECONDS_HF21;
   result["freezone_ROOT_POST_PARENT"] = freezone_ROOT_POST_PARENT;
   result["freezone_SAVINGS_WITHDRAW_REQUEST_LIMIT"] = freezone_SAVINGS_WITHDRAW_REQUEST_LIMIT;
   result["freezone_SAVINGS_WITHDRAW_TIME"] = freezone_SAVINGS_WITHDRAW_TIME;
   result["freezone_SBD_START_PERCENT_HF14"] = freezone_SBD_START_PERCENT_HF14;
   result["freezone_SBD_START_PERCENT_HF20"] = freezone_SBD_START_PERCENT_HF20;
   result["freezone_SBD_STOP_PERCENT_HF14"] = freezone_SBD_STOP_PERCENT_HF14;
   result["freezone_SBD_STOP_PERCENT_HF20"] = freezone_SBD_STOP_PERCENT_HF20;
   result["freezone_SECOND_CASHOUT_WINDOW"] = freezone_SECOND_CASHOUT_WINDOW;
   result["freezone_SOFT_MAX_COMMENT_DEPTH"] = freezone_SOFT_MAX_COMMENT_DEPTH;
   result["freezone_START_MINER_VOTING_BLOCK"] = freezone_START_MINER_VOTING_BLOCK;
   result["freezone_START_VESTING_BLOCK"] = freezone_START_VESTING_BLOCK;
   result["freezone_TEMP_ACCOUNT"] = freezone_TEMP_ACCOUNT;
   result["freezone_UPVOTE_LOCKOUT_HF7"] = freezone_UPVOTE_LOCKOUT_HF7;
   result["freezone_UPVOTE_LOCKOUT_HF17"] = freezone_UPVOTE_LOCKOUT_HF17;
   result["freezone_UPVOTE_LOCKOUT_SECONDS"] = freezone_UPVOTE_LOCKOUT_SECONDS;
   result["freezone_VESTING_FUND_PERCENT_HF16"] = freezone_VESTING_FUND_PERCENT_HF16;
   result["freezone_VESTING_WITHDRAW_INTERVALS"] = freezone_VESTING_WITHDRAW_INTERVALS;
   result["freezone_VESTING_WITHDRAW_INTERVALS_PRE_HF_16"] = freezone_VESTING_WITHDRAW_INTERVALS_PRE_HF_16;
   result["freezone_VESTING_WITHDRAW_INTERVAL_SECONDS"] = freezone_VESTING_WITHDRAW_INTERVAL_SECONDS;
   result["freezone_VOTE_DUST_THRESHOLD"] = freezone_VOTE_DUST_THRESHOLD;
   result["freezone_VOTING_MANA_REGENERATION_SECONDS"] = freezone_VOTING_MANA_REGENERATION_SECONDS;
   result["freezone_SYMBOL"] = freezone_SYMBOL;
   result["VESTS_SYMBOL"] = VESTS_SYMBOL;
   result["freezone_VIRTUAL_SCHEDULE_LAP_LENGTH"] = freezone_VIRTUAL_SCHEDULE_LAP_LENGTH;
   result["freezone_VIRTUAL_SCHEDULE_LAP_LENGTH2"] = freezone_VIRTUAL_SCHEDULE_LAP_LENGTH2;
   result["freezone_VOTES_PER_PERIOD_SST_HF"] = freezone_VOTES_PER_PERIOD_SST_HF;
   result["freezone_MAX_LIMIT_ORDER_EXPIRATION"] = freezone_MAX_LIMIT_ORDER_EXPIRATION;
   result["freezone_DELEGATION_RETURN_PERIOD_HF0"] = freezone_DELEGATION_RETURN_PERIOD_HF0;
   result["freezone_DELEGATION_RETURN_PERIOD_HF20"] = freezone_DELEGATION_RETURN_PERIOD_HF20;
   result["freezone_RD_MIN_DECAY_BITS"] = freezone_RD_MIN_DECAY_BITS;
   result["freezone_RD_MAX_DECAY_BITS"] = freezone_RD_MAX_DECAY_BITS;
   result["freezone_RD_DECAY_DENOM_SHIFT"] = freezone_RD_DECAY_DENOM_SHIFT;
   result["freezone_RD_MAX_POOL_BITS"] = freezone_RD_MAX_POOL_BITS;
   result["freezone_RD_MAX_BUDGET_1"] = freezone_RD_MAX_BUDGET_1;
   result["freezone_RD_MAX_BUDGET_2"] = freezone_RD_MAX_BUDGET_2;
   result["freezone_RD_MAX_BUDGET_3"] = freezone_RD_MAX_BUDGET_3;
   result["freezone_RD_MAX_BUDGET"] = freezone_RD_MAX_BUDGET;
   result["freezone_RD_MIN_DECAY"] = freezone_RD_MIN_DECAY;
   result["freezone_RD_MIN_BUDGET"] = freezone_RD_MIN_BUDGET;
   result["freezone_RD_MAX_DECAY"] = freezone_RD_MAX_DECAY;
   result["freezone_ACCOUNT_SUBSIDY_PRECISION"] = freezone_ACCOUNT_SUBSIDY_PRECISION;
   result["freezone_WITNESS_SUBSIDY_BUDGET_PERCENT"] = freezone_WITNESS_SUBSIDY_BUDGET_PERCENT;
   result["freezone_WITNESS_SUBSIDY_DECAY_PERCENT"] = freezone_WITNESS_SUBSIDY_DECAY_PERCENT;
   result["freezone_DEFAULT_ACCOUNT_SUBSIDY_DECAY"] = freezone_DEFAULT_ACCOUNT_SUBSIDY_DECAY;
   result["freezone_DEFAULT_ACCOUNT_SUBSIDY_BUDGET"] = freezone_DEFAULT_ACCOUNT_SUBSIDY_BUDGET;
   result["freezone_DECAY_BACKSTOP_PERCENT"] = freezone_DECAY_BACKSTOP_PERCENT;
   result["freezone_BLOCK_GENERATION_POSTPONED_TX_LIMIT"] = freezone_BLOCK_GENERATION_POSTPONED_TX_LIMIT;
   result["freezone_PENDING_TRANSACTION_EXECUTION_LIMIT"] = freezone_PENDING_TRANSACTION_EXECUTION_LIMIT;
   result["freezone_TREASURY_ACCOUNT"] = freezone_TREASURY_ACCOUNT;
   result["freezone_TREASURY_FEE"] = freezone_TREASURY_FEE;
   result["freezone_PROPOSAL_MAINTENANCE_PERIOD"] = freezone_PROPOSAL_MAINTENANCE_PERIOD;
   result["freezone_PROPOSAL_MAINTENANCE_CLEANUP"] = freezone_PROPOSAL_MAINTENANCE_CLEANUP;
   result["freezone_PROPOSAL_SUBJECT_MAX_LENGTH"] = freezone_PROPOSAL_SUBJECT_MAX_LENGTH;
   result["freezone_PROPOSAL_MAX_IDS_NUMBER"] = freezone_PROPOSAL_MAX_IDS_NUMBER;
   result["freezone_NETWORK_TYPE"] = freezone_NETWORK_TYPE;
   result["freezone_DB_FORMAT_VERSION"] = freezone_DB_FORMAT_VERSION;

   return result;
}

} } // freezone::protocol
