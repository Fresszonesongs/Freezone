#pragma once
#include <freezone/plugins/json_rpc/utility.hpp>

#include <freezone/protocol/types.hpp>

#include <fc/optional.hpp>

namespace freezone { namespace plugins { namespace chain {

namespace detail { class chain_api_impl; }

struct push_block_args
{
   freezone::chain::signed_block block;
   bool                         currently_syncing = false;
};


struct push_block_return
{
   bool              success;
   optional<string>  error;
};

typedef freezone::chain::signed_transaction push_transaction_args;

struct push_transaction_return
{
   bool              success;
   optional<string>  error;
};


class chain_api
{
   public:
      chain_api();
      ~chain_api();

      DECLARE_API(
         (push_block)
         (push_transaction) )
      
   private:
      std::unique_ptr< detail::chain_api_impl > my;
};

} } } // freezone::plugins::chain

FC_REFLECT( freezone::plugins::chain::push_block_args, (block)(currently_syncing) )
FC_REFLECT( freezone::plugins::chain::push_block_return, (success)(error) )
FC_REFLECT( freezone::plugins::chain::push_transaction_return, (success)(error) )
