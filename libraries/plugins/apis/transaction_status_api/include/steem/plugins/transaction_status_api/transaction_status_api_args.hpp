#pragma once

#include <freezone/protocol/types.hpp>
#include <freezone/protocol/transaction.hpp>
#include <freezone/protocol/block_header.hpp>

#include <freezone/plugins/json_rpc/utility.hpp>
#include <freezone/plugins/transaction_status/transaction_status_objects.hpp>

namespace freezone { namespace plugins { namespace transaction_status_api {

struct find_transaction_args
{
   chain::transaction_id_type transaction_id;
   fc::optional< fc::time_point_sec > expiration;
};

struct find_transaction_return
{
   transaction_status::transaction_status status;
   fc::optional< uint32_t > block_num;
};

} } } // freezone::transaction_status_api

FC_REFLECT( freezone::plugins::transaction_status_api::find_transaction_args, (transaction_id)(expiration) )
FC_REFLECT( freezone::plugins::transaction_status_api::find_transaction_return, (status)(block_num) )
