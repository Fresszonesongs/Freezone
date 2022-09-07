#pragma once

#include <fc/container/flat.hpp>
#include <freezone/protocol/operations.hpp>
#include <freezone/protocol/transaction.hpp>

#include <fc/string.hpp>

namespace freezone { namespace app {

using namespace fc;

void operation_get_impacted_accounts(
   const freezone::protocol::operation& op,
   fc::flat_set<protocol::account_name_type>& result );

void transaction_get_impacted_accounts(
   const freezone::protocol::transaction& tx,
   fc::flat_set<protocol::account_name_type>& result
   );

} } // freezone::app
