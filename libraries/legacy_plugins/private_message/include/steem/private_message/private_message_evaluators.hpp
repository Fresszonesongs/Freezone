#pragma once

#include <freezone/chain/evaluator.hpp>

#include <freezone/private_message/private_message_operations.hpp>
#include <freezone/private_message/private_message_plugin.hpp>

namespace freezone { namespace private_message {

freezone_DEFINE_PLUGIN_EVALUATOR( private_message_plugin, freezone::private_message::private_message_plugin_operation, private_message )

} }
