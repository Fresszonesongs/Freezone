#pragma once

#include <freezone/protocol/freezone_required_actions.hpp>

#include <freezone/protocol/operation_util.hpp>

namespace freezone { namespace protocol {

   /** NOTE: do not change the order of any actions or it will trigger a hardfork.
    */
   typedef fc::static_variant<
            SST_ico_launch_action,
            SST_ico_evaluation_action,
            SST_token_launch_action,
            SST_refund_action,
            SST_contributor_payout_action,
            SST_founder_payout_action
#ifdef IS_TEST_NET
            ,example_required_action
#endif
         > required_automated_action;

} } // freezone::protocol

freezone_DECLARE_OPERATION_TYPE( freezone::protocol::required_automated_action );

FC_REFLECT_TYPENAME( freezone::protocol::required_automated_action );
