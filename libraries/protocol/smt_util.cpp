#include <freezone/protocol/SST_util.hpp>
#include <freezone/protocol/authority.hpp>

namespace freezone { namespace protocol { namespace SST {

namespace unit_target {

bool is_contributor( const unit_target_type& unit_target )
{
   return unit_target == SST_DESTINATION_FROM || unit_target == SST_DESTINATION_FROM_VESTING;
}

bool is_market_maker( const unit_target_type& unit_target )
{
   return unit_target == SST_DESTINATION_MARKET_MAKER;
}

bool is_rewards( const unit_target_type& unit_target )
{
   return unit_target == SST_DESTINATION_REWARDS;
}

bool is_founder_vesting( const unit_target_type& unit_target )
{
   std::string unit_target_str = unit_target;
   if ( unit_target_str.size() > std::strlen( SST_DESTINATION_ACCOUNT_PREFIX ) + std::strlen( SST_DESTINATION_VESTING_SUFFIX ) )
   {
      auto pos = unit_target_str.find( SST_DESTINATION_ACCOUNT_PREFIX );
      if ( pos != std::string::npos && pos == 0 )
      {
         std::size_t suffix_len = std::strlen( SST_DESTINATION_VESTING_SUFFIX );
         if ( !unit_target_str.compare( unit_target_str.size() - suffix_len, suffix_len, SST_DESTINATION_VESTING_SUFFIX ) )
         {
            return true;
         }
      }
   }

   return false;
}

bool is_vesting( const unit_target_type& unit_target )
{
   return unit_target == SST_DESTINATION_VESTING;
}

bool is_account_name_type( const unit_target_type& unit_target )
{
   if ( is_contributor( unit_target ) )
      return false;
   if ( is_rewards( unit_target ) )
      return false;
   if ( is_market_maker( unit_target ) )
      return false;
   if ( is_vesting( unit_target ) )
      return false;
   return true;
}

bool is_vesting_type( const unit_target_type& unit_target )
{
   if ( unit_target == SST_DESTINATION_FROM_VESTING )
      return true;
   if ( unit_target == SST_DESTINATION_VESTING )
      return true;
   if ( is_founder_vesting( unit_target ) )
      return true;

   return false;
}

account_name_type get_unit_target_account( const unit_target_type& unit_target )
{
   FC_ASSERT( !is_contributor( unit_target ),  "Cannot derive an account name from a contributor special destination." );
   FC_ASSERT( !is_market_maker( unit_target ), "The market maker unit target is not a valid account." );
   FC_ASSERT( !is_rewards( unit_target ),      "The rewards unit target is not a valid account." );

   if ( is_valid_account_name( unit_target ) )
      return account_name_type( unit_target );

   // This is a special unit target destination in the form of $alice.vesting
   FC_ASSERT( unit_target.size() > std::strlen( SST_DESTINATION_ACCOUNT_PREFIX ) + std::strlen( SST_DESTINATION_VESTING_SUFFIX ),
      "Unit target '${target}' is malformed", ("target", unit_target) );

   std::string str_name = unit_target;
   auto pos = str_name.find( SST_DESTINATION_ACCOUNT_PREFIX );
   FC_ASSERT( pos != std::string::npos && pos == 0, "Expected SST destination account prefix '${prefix}' for unit target '${target}'.",
      ("prefix", SST_DESTINATION_ACCOUNT_PREFIX)("target", unit_target) );

   std::size_t suffix_len = std::strlen( SST_DESTINATION_VESTING_SUFFIX );
   FC_ASSERT( !str_name.compare( str_name.size() - suffix_len, suffix_len, SST_DESTINATION_VESTING_SUFFIX ),
      "Expected SST destination vesting suffix '${suffix}' for unit target '${target}'.",
         ("suffix", SST_DESTINATION_VESTING_SUFFIX)("target", unit_target) );

   std::size_t prefix_len = std::strlen( SST_DESTINATION_ACCOUNT_PREFIX );
   account_name_type unit_target_account = str_name.substr( prefix_len, str_name.size() - suffix_len - prefix_len );
   FC_ASSERT( is_valid_account_name( unit_target_account ), "The derived unit target account name '${name}' is invalid.",
      ("name", unit_target_account) );

   return unit_target_account;
}

bool is_valid_emissions_destination( const unit_target_type& unit_target )
{
   if ( is_market_maker( unit_target ) )
      return true;
   if ( is_rewards( unit_target ) )
      return true;
   if ( is_vesting( unit_target ) )
      return true;
   if ( is_valid_account_name( unit_target ) )
      return true;
   if ( is_founder_vesting( unit_target ) )
      return true;
   return false;
}

} // freezone::protocol::SST::unit_target

} } } // freezone::protocol::SST
