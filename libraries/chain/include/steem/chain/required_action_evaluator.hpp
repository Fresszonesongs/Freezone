#pragma once

#include <freezone/protocol/freezone_required_actions.hpp>

#include <freezone/chain/evaluator.hpp>

namespace freezone { namespace chain {

using namespace freezone::protocol;

#ifdef IS_TEST_NET
freezone_DEFINE_ACTION_EVALUATOR( example_required, required_automated_action )
#endif

freezone_DEFINE_ACTION_EVALUATOR( SST_ico_launch, required_automated_action )
freezone_DEFINE_ACTION_EVALUATOR( SST_ico_evaluation, required_automated_action )
freezone_DEFINE_ACTION_EVALUATOR( SST_token_launch, required_automated_action )
freezone_DEFINE_ACTION_EVALUATOR( SST_refund, required_automated_action )
freezone_DEFINE_ACTION_EVALUATOR( SST_contributor_payout, required_automated_action )
freezone_DEFINE_ACTION_EVALUATOR( SST_founder_payout, required_automated_action )

} } //freezone::chain
