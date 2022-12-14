#pragma once

#include <freezone/chain/util/asset.hpp>

#include <freezone/protocol/asset.hpp>
#include <freezone/protocol/config.hpp>
#include <freezone/protocol/types.hpp>
#include <freezone/protocol/misc_utilities.hpp>

#include <fc/reflect/reflect.hpp>

#include <fc/uint128.hpp>

namespace freezone { namespace chain { namespace util {

using freezone::protocol::asset;
using freezone::protocol::price;
using freezone::protocol::share_type;

using fc::uint128_t;

struct comment_reward_context
{
   share_type           rshares;
   uint16_t             reward_weight = 0;
   uint128_t            total_claims;
   share_type           reward_fund;
   protocol::curve_id   reward_curve = protocol::quadratic;
   uint128_t            content_constant = freezone_CONTENT_CONSTANT_HF0;
};

uint64_t get_rshare_reward( const comment_reward_context& ctx );

inline uint128_t get_content_constant_s()
{
   return freezone_CONTENT_CONSTANT_HF0; // looking good for posters
}

uint128_t evaluate_reward_curve( const uint128_t& rshares, const protocol::curve_id& curve = protocol::quadratic, const uint128_t& var1 = freezone_CONTENT_CONSTANT_HF0 );

} } } // freezone::chain::util

FC_REFLECT( freezone::chain::util::comment_reward_context,
   (rshares)
   (reward_weight)
   (total_claims)
   (reward_fund)
   (reward_curve)
   (content_constant)
   )
