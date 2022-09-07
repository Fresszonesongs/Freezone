#pragma once
#include <freezone/protocol/block_header.hpp>
#include <freezone/protocol/transaction.hpp>

namespace freezone { namespace protocol {

   struct signed_block : public signed_block_header
   {
      checksum_type calculate_merkle_root()const;
      vector<signed_transaction> transactions;
   };

} } // freezone::protocol

FC_REFLECT_DERIVED( freezone::protocol::signed_block, (freezone::protocol::signed_block_header), (transactions) )
