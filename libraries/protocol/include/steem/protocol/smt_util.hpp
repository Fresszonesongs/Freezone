#pragma once
#include <string>
#include <freezone/protocol/types.hpp>

#define SST_DESTINATION_PREFIX         "$"
#define SST_DESTINATION_ACCOUNT_PREFIX SST_DESTINATION_PREFIX "!"
#define SST_DESTINATION_VESTING_SUFFIX ".vesting"
#define SST_DESTINATION_FROM           unit_target_type( SST_DESTINATION_PREFIX "from" )
#define SST_DESTINATION_FROM_VESTING   unit_target_type( SST_DESTINATION_PREFIX "from" SST_DESTINATION_VESTING_SUFFIX )
#define SST_DESTINATION_MARKET_MAKER   unit_target_type( SST_DESTINATION_PREFIX "market_maker" )
#define SST_DESTINATION_REWARDS        unit_target_type( SST_DESTINATION_PREFIX "rewards" )
#define SST_DESTINATION_VESTING        unit_target_type( SST_DESTINATION_PREFIX "vesting" )

#define SST_DESTINATION_ACCOUNT_VESTING(name) unit_target_type( SST_DESTINATION_ACCOUNT_PREFIX name SST_DESTINATION_VESTING_SUFFIX )

namespace freezone { namespace protocol { namespace SST {

namespace unit_target {

bool is_contributor( const unit_target_type& unit_target );
bool is_market_maker( const unit_target_type& unit_target );
bool is_rewards( const unit_target_type& unit_target );
bool is_founder_vesting( const unit_target_type& unit_target );
bool is_vesting( const unit_target_type& unit_target );

bool is_account_name_type( const unit_target_type& unit_target );
bool is_vesting_type( const unit_target_type& unit_target );

account_name_type get_unit_target_account( const unit_target_type& unit_target );

bool is_valid_emissions_destination( const unit_target_type& unit_target );

} // freezone::protocol::SST::unit_target

} } } // freezone::protocol::SST
