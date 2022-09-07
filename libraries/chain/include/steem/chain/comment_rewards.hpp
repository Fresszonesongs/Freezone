#pragma once

#include <freezone/chain/freezone_fwd.hpp>
#include <freezone/chain/database.hpp>

namespace freezone { namespace chain {

struct reward_fund_context
{
   uint128_t            recent_claims = 0;
   asset                reward_balance = asset( 0, freezone_SYMBOL );
   time_point_sec       last_update;
   share_type           tokens_awarded = 0;
   protocol::curve_id   author_reward_curve;
   protocol::curve_id   curation_reward_curve;
   uint128_t            content_constant;
   uint16_t             percent_curation_rewards = 0;
};

void process_comment_rewards( database& db );

} } // freezone::chain

FC_REFLECT( freezone::chain::reward_fund_context,
            (recent_claims)
            (reward_balance)
            (last_update)
            (tokens_awarded)
            (author_reward_curve)
            (curation_reward_curve)
            (content_constant)
            (percent_curation_rewards)
         )
