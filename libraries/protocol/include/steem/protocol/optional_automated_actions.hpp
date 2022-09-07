#pragma once

#include <freezone/protocol/freezone_optional_actions.hpp>

#include <freezone/protocol/operation_util.hpp>

namespace freezone { namespace protocol {

   /** NOTE: do not change the order of any actions or it will trigger a hardfork.
    */
   typedef fc::static_variant<
            SST_token_emission_action
#ifdef IS_TEST_NET
            ,example_optional_action
#endif
         > optional_automated_action;

} } // freezone::protocol

freezone_DECLARE_OPERATION_TYPE( freezone::protocol::optional_automated_action );

FC_REFLECT_TYPENAME( freezone::protocol::optional_automated_action );
