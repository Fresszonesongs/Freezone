#pragma once

#include <fc/time.hpp>

#include <freezone/chain/database.hpp>

namespace freezone { namespace plugins { namespace chain {
   
class abstract_block_producer {
public:
   virtual ~abstract_block_producer() = default;

   virtual freezone::chain::signed_block generate_block(
      fc::time_point_sec when,
      const freezone::chain::account_name_type& witness_owner,
      const fc::ecc::private_key& block_signing_private_key,
      uint32_t skip = freezone::chain::database::skip_nothing) = 0;
};

} } } // freezone::plugins::chain
