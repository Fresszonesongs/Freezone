#include <freezone/protocol/validation.hpp>
#include <freezone/protocol/freezone_optional_actions.hpp>
#include <freezone/protocol/SST_util.hpp>

namespace freezone { namespace protocol {

#ifdef IS_TEST_NET
void example_optional_action::validate()const
{
   validate_account_name( account );
}
#endif

void SST_token_emission_action::validate() const
{
   validate_account_name( control_account );
   validate_SST_symbol( symbol );
   FC_ASSERT( emissions.empty() == false, "Emissions cannot be empty" );
   for ( const auto& e : emissions )
   {
      FC_ASSERT( SST::unit_target::is_valid_emissions_destination( e.first ),
         "Emissions destination ${n} is invalid", ("n", e.first) );
      FC_ASSERT( e.second > 0, "Emissions must be greater than 0" );
   }
}

} } //freezone::protocol
