#pragma once

#include <freezone/protocol/block.hpp>

namespace freezone { namespace chain {

struct transaction_notification
{
   transaction_notification( const freezone::protocol::signed_transaction& tx ) : transaction(tx)
   {
      transaction_id = tx.id();
   }

   freezone::protocol::transaction_id_type          transaction_id;
   const freezone::protocol::signed_transaction&    transaction;
};

} }
