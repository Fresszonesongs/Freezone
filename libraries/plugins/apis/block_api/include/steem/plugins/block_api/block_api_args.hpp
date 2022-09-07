#pragma once
#include <freezone/plugins/block_api/block_api_objects.hpp>

#include <freezone/protocol/types.hpp>
#include <freezone/protocol/transaction.hpp>
#include <freezone/protocol/block_header.hpp>

#include <freezone/plugins/json_rpc/utility.hpp>

namespace freezone { namespace plugins { namespace block_api {

/* get_block_header */

struct get_block_header_args
{
   uint32_t block_num;
};

struct get_block_header_return
{
   optional< block_header > header;
};

/* get_block */
struct get_block_args
{
   uint32_t block_num;
};

struct get_block_return
{
   optional< api_signed_block_object > block;
};

} } } // freezone::block_api

FC_REFLECT( freezone::plugins::block_api::get_block_header_args,
   (block_num) )

FC_REFLECT( freezone::plugins::block_api::get_block_header_return,
   (header) )

FC_REFLECT( freezone::plugins::block_api::get_block_args,
   (block_num) )

FC_REFLECT( freezone::plugins::block_api::get_block_return,
   (block) )

