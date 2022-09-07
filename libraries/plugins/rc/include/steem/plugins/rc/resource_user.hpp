#pragma once

#include <freezone/protocol/types.hpp>

#include <fc/reflect/reflect.hpp>

namespace freezone { namespace protocol {
struct signed_transaction;
} } // freezone::protocol

namespace freezone { namespace plugins { namespace rc {

using freezone::protocol::account_name_type;
using freezone::protocol::signed_transaction;

account_name_type get_resource_user( const signed_transaction& tx );

} } } // freezone::plugins::rc
