#pragma once

#include <freezone/protocol/base.hpp>
#include <freezone/protocol/asset.hpp>
#include <freezone/protocol/misc_utilities.hpp>

#define SST_MAX_UNIT_ROUTES            10
#define SST_MAX_UNIT_COUNT             20
#define SST_MAX_DECIMAL_PLACES         8

namespace freezone { namespace protocol {

/**
 * This operation introduces new SST into blockchain as identified by
 * Numerical Asset Identifier (NAI). Also the SST precision (decimal points)
 * is explicitly provided.
 */
struct SST_create_operation : public base_operation
{
   account_name_type control_account;
   asset_symbol_type symbol;

   SST_ticker_type   desired_ticker;

   /// The amount to be transfered from @account to null account as elevation fee.
   asset             SST_creation_fee;
   /// Separately provided precision for clarity and redundancy.
   uint8_t           precision;

   extensions_type   extensions;

   void validate()const;

   void get_required_active_authorities( flat_set<account_name_type>& a )const
   { a.insert( control_account ); }
};

struct SST_generation_unit
{
   flat_map< unit_target_type, uint16_t > freezone_unit;
   flat_map< unit_target_type, uint16_t > token_unit;

   uint32_t freezone_unit_sum()const;
   uint32_t token_unit_sum()const;

   void validate()const;
};

struct SST_capped_generation_policy
{
   SST_generation_unit generation_unit;

   extensions_type     extensions;

   void validate()const;
};

typedef static_variant<
   SST_capped_generation_policy
   > SST_generation_policy;

struct SST_setup_operation : public base_operation
{
   account_name_type control_account;
   asset_symbol_type symbol;

   int64_t                 max_supply = freezone_MAX_SHARE_SUPPLY;

   time_point_sec          contribution_begin_time;
   time_point_sec          contribution_end_time;
   time_point_sec          launch_time;

   share_type              freezone_satoshi_min;

   uint32_t                min_unit_ratio = 0;
   uint32_t                max_unit_ratio = 0;

   extensions_type         extensions;

   void validate()const;

   void get_required_active_authorities( flat_set<account_name_type>& a )const
   { a.insert( control_account ); }
};

struct SST_emissions_unit
{
   flat_map< unit_target_type, uint16_t > token_unit;

   void validate()const;
   uint32_t token_unit_sum()const;
};

struct SST_setup_ico_tier_operation : public base_operation
{
   account_name_type     control_account;
   asset_symbol_type     symbol;

   share_type            freezone_satoshi_cap;
   SST_generation_policy generation_policy;
   bool                  remove = false;

   extensions_type       extensions;

   void validate()const;

   void get_required_active_authorities( flat_set<account_name_type>& a )const
   { a.insert( control_account ); }
};

struct SST_setup_emissions_operation : public base_operation
{
   account_name_type   control_account;
   asset_symbol_type   symbol;

   time_point_sec      schedule_time;
   SST_emissions_unit  emissions_unit;

   uint32_t            interval_seconds = 0;
   uint32_t            emission_count = 0;

   time_point_sec      lep_time;
   time_point_sec      rep_time;

   share_type          lep_abs_amount;
   share_type          rep_abs_amount;
   uint32_t            lep_rel_amount_numerator = 0;
   uint32_t            rep_rel_amount_numerator = 0;

   uint8_t             rel_amount_denom_bits = 0;
   bool                remove = false;
   bool                floor_emissions = false;

   extensions_type     extensions;

   void validate()const;

   void get_required_active_authorities( flat_set<account_name_type>& a )const
   { a.insert( control_account ); }
};

struct SST_param_allow_voting
{
   bool value = true;
};

typedef static_variant<
   SST_param_allow_voting
   > SST_setup_parameter;

struct SST_param_windows_v1
{
   uint32_t cashout_window_seconds = 0;                // freezone_CASHOUT_WINDOW_SECONDS
   uint32_t reverse_auction_window_seconds = 0;        // freezone_REVERSE_AUCTION_WINDOW_SECONDS
};

struct SST_param_vote_regeneration_period_seconds_v1
{
   uint32_t vote_regeneration_period_seconds = 0;      // freezone_VOTING_MANA_REGENERATION_SECONDS
   uint32_t votes_per_regeneration_period = 0;
};

struct SST_param_rewards_v1
{
   uint128_t               content_constant = 0;
   uint16_t                percent_curation_rewards = 0;
   protocol::curve_id      author_reward_curve;
   protocol::curve_id      curation_reward_curve;
};

struct SST_param_allow_downvotes
{
   bool value = true;
};

typedef static_variant<
   SST_param_windows_v1,
   SST_param_vote_regeneration_period_seconds_v1,
   SST_param_rewards_v1,
   SST_param_allow_downvotes
   > SST_runtime_parameter;

struct SST_set_setup_parameters_operation : public base_operation
{
   account_name_type                control_account;
   asset_symbol_type                symbol;
   flat_set< SST_setup_parameter >  setup_parameters;
   extensions_type                  extensions;

   void validate()const;

   void get_required_active_authorities( flat_set<account_name_type>& a )const
   { a.insert( control_account ); }
};

struct SST_set_runtime_parameters_operation : public base_operation
{
   account_name_type                   control_account;
   asset_symbol_type                   symbol;
   flat_set< SST_runtime_parameter >   runtime_parameters;
   extensions_type                     extensions;

   void validate()const;

   void get_required_active_authorities( flat_set<account_name_type>& a )const
   { a.insert( control_account ); }
};

struct SST_contribute_operation : public base_operation
{
   account_name_type  contributor;
   asset_symbol_type  symbol;
   uint32_t           contribution_id;
   asset              contribution;
   extensions_type    extensions;

   void validate() const;
   void get_required_active_authorities( flat_set<account_name_type>& a )const
   { a.insert( contributor ); }
};

} }

FC_REFLECT(
   freezone::protocol::SST_create_operation,
   (control_account)
   (symbol)
   (desired_ticker)
   (SST_creation_fee)
   (precision)
   (extensions)
)

FC_REFLECT(
   freezone::protocol::SST_setup_operation,
   (control_account)
   (symbol)
   (max_supply)
   (contribution_begin_time)
   (contribution_end_time)
   (launch_time)
   (freezone_satoshi_min)
   (min_unit_ratio)
   (max_unit_ratio)
   (extensions)
   )

FC_REFLECT(
   freezone::protocol::SST_generation_unit,
   (freezone_unit)
   (token_unit)
   )


FC_REFLECT(
   freezone::protocol::SST_capped_generation_policy,
   (generation_unit)
   (extensions)
   )

FC_REFLECT(
   freezone::protocol::SST_emissions_unit,
   (token_unit)
   )

FC_REFLECT(
   freezone::protocol::SST_setup_ico_tier_operation,
   (control_account)
   (symbol)
   (freezone_satoshi_cap)
   (generation_policy)
   (remove)
   (extensions)
   )

FC_REFLECT(
   freezone::protocol::SST_setup_emissions_operation,
   (control_account)
   (symbol)
   (schedule_time)
   (emissions_unit)
   (interval_seconds)
   (emission_count)
   (lep_time)
   (rep_time)
   (lep_abs_amount)
   (rep_abs_amount)
   (lep_rel_amount_numerator)
   (rep_rel_amount_numerator)
   (rel_amount_denom_bits)
   (remove)
   (floor_emissions)
   (extensions)
   )

FC_REFLECT(
   freezone::protocol::SST_param_allow_voting,
   (value)
   )

FC_REFLECT_TYPENAME( freezone::protocol::SST_setup_parameter )

FC_REFLECT(
   freezone::protocol::SST_param_windows_v1,
   (cashout_window_seconds)
   (reverse_auction_window_seconds)
   )

FC_REFLECT(
   freezone::protocol::SST_param_vote_regeneration_period_seconds_v1,
   (vote_regeneration_period_seconds)
   (votes_per_regeneration_period)
   )

FC_REFLECT(
   freezone::protocol::SST_param_rewards_v1,
   (content_constant)
   (percent_curation_rewards)
   (author_reward_curve)
   (curation_reward_curve)
   )

FC_REFLECT(
   freezone::protocol::SST_param_allow_downvotes,
   (value)
)

FC_REFLECT_TYPENAME(
   freezone::protocol::SST_runtime_parameter
   )

FC_REFLECT(
   freezone::protocol::SST_set_setup_parameters_operation,
   (control_account)
   (symbol)
   (setup_parameters)
   (extensions)
   )

FC_REFLECT(
   freezone::protocol::SST_set_runtime_parameters_operation,
   (control_account)
   (symbol)
   (runtime_parameters)
   (extensions)
   )

FC_REFLECT(
   freezone::protocol::SST_contribute_operation,
   (contributor)
   (symbol)
   (contribution_id)
   (contribution)
   (extensions)
   )
