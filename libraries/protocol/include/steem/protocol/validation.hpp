
#pragma once

#include <freezone/protocol/base.hpp>
#include <freezone/protocol/block_header.hpp>
#include <freezone/protocol/asset.hpp>

#include <fc/utf8.hpp>

namespace freezone { namespace protocol {

inline bool is_asset_type( asset asset, asset_symbol_type symbol )
{
   return asset.symbol == symbol;
}

inline void validate_account_name( const string& name )
{
   FC_ASSERT( is_valid_account_name( name ), "Account name ${n} is invalid", ("n", name) );
}

inline void validate_permlink( const string& permlink )
{
   FC_ASSERT( permlink.size() < freezone_MAX_PERMLINK_LENGTH, "permlink is too long" );
   FC_ASSERT( fc::is_utf8( permlink ), "permlink not formatted in UTF8" );
}

inline void validate_SST_symbol( const asset_symbol_type& symbol )
{
   symbol.validate();
   FC_ASSERT( symbol.space() == asset_symbol_type::SST_nai_space, "legacy symbol used instead of NAI" );
   FC_ASSERT( symbol.is_vesting() == false, "liquid variant of NAI expected");
}

/**
 * Determine if a price complies with Tick Pricing Rules.
 *
 * - For prices involving freezone Dollars (SBD), the base asset must be SBD.
 * - For prices involving SST assets, the base asset must be freezone.
 * - The quote must be a power of 10.
 *
 * \param  price The price to check for Tick Pricing compliance
 * \return true  If price conforms to Tick Pricing Rules
 */
inline bool is_tick_pricing( const price& p )
{
   if ( p.base.symbol == SBD_SYMBOL )
   {
      if ( p.quote.symbol != freezone_SYMBOL )
         return false;
   }
   else if ( p.base.symbol == freezone_SYMBOL )
   {
      if ( p.quote.symbol.space() != asset_symbol_type::SST_nai_space )
         return false;

      if ( p.quote.symbol.is_vesting() )
         return false;
   }
   else
   {
      return false;
   }

   share_type tmp = p.quote.amount;
   while ( tmp > 9 && tmp % 10 == 0 )
      tmp /= 10;

   if ( tmp != 1 )
      return false;

   return true;
}

/**
 * Validate a price complies with Tick Pricing Rules.
 *
 * - For prices involving freezone Dollars (SBD), the base asset must be SBD.
 * - For prices involving SST assets, the base asset must be freezone.
 * - The quote must be a power of 10.
 *
 * \param price The price to perform Tick Pricing Rules validation
 * \throw fc::assert_exception If the price does not conform to Tick Pricing Rules
 */
inline void validate_tick_pricing( const price& p )
{
   if ( p.base.symbol == SBD_SYMBOL )
   {
      FC_ASSERT( p.quote.symbol == freezone_SYMBOL, "Only freezone can be paired with SBD as the base symbol." );
   }
   else if ( p.base.symbol == freezone_SYMBOL )
   {
      FC_ASSERT( p.quote.symbol.space() == asset_symbol_type::SST_nai_space, "Only tokens can be paired with freezone as the base symbol." );
      FC_ASSERT( p.quote.symbol.is_vesting() == false, "Quote symbol cannot be a vesting symbol." );
   }
   else
   {
      FC_ASSERT( false, "freezone and SBD are the only valid base symbols." );
   }

   share_type tmp = p.quote.amount;
   while ( tmp > 9 && tmp % 10 == 0 )
      tmp /= 10;
   FC_ASSERT( tmp == 1, "The quote amount must be a power of 10." );
}

} }
