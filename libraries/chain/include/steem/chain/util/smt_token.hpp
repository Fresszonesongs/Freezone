#pragma once

#include <fc/optional.hpp>
#include <freezone/chain/database.hpp>
#include <freezone/chain/SST_objects.hpp>
#include <freezone/protocol/asset_symbol.hpp>

namespace freezone { namespace chain { namespace util { namespace SST {

const SST_token_object* find_token( const database& db, uint32_t nai );
const SST_token_object* find_token( const database& db, asset_symbol_type symbol, bool precision_agnostic = false );
const SST_token_emissions_object* last_emission( const database& db, const asset_symbol_type& symbol );
fc::optional< time_point_sec > last_emission_time( const database& db, const asset_symbol_type& symbol );
fc::optional< time_point_sec > next_emission_time( const database& db, const asset_symbol_type& symbol, time_point_sec time = time_point_sec() );
const SST_token_emissions_object* get_emission_object( const database& db, const asset_symbol_type& symbol, time_point_sec t );
flat_map< unit_target_type, share_type > generate_emissions( const SST_token_object& token, const SST_token_emissions_object& emission, time_point_sec t );

namespace ico {

bool schedule_next_refund( database& db, const asset_symbol_type& a );
bool schedule_next_contributor_payout( database& db, const asset_symbol_type& a );
bool schedule_founder_payout( database& db, const asset_symbol_type& a );

share_type payout( database& db, const asset_symbol_type& symbol, const account_object& account, const std::vector< contribution_payout >& payouts );
fc::optional< share_type > get_ico_freezone_hard_cap( database& db, const asset_symbol_type& a );
std::size_t ico_tier_size( database& db, const asset_symbol_type& symbol );
void remove_ico_objects( database& db, const asset_symbol_type& symbol );

} // freezone::chain::util::SST::ico

} } } } // freezone::chain::util::SST
