#pragma once

#include <freezone/protocol/freezone_optional_actions.hpp>

#include <freezone/chain/evaluator.hpp>

namespace freezone { namespace chain {

using namespace freezone::protocol;

#ifdef IS_TEST_NET
freezone_DEFINE_ACTION_EVALUATOR( example_optional, optional_automated_action )
#endif

freezone_DEFINE_ACTION_EVALUATOR( SST_token_emission, optional_automated_action )

} } //freezone::chain
