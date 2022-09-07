#pragma once

#include <freezone/plugins/rewards_api/rewards_api_args.hpp>
#include <freezone/plugins/json_rpc/utility.hpp>

namespace freezone { namespace plugins { namespace rewards_api {

namespace detail { class rewards_api_impl; }

class rewards_api
{
public:
   rewards_api();
   ~rewards_api();

   DECLARE_API(
      (simulate_curve_payouts)
   );
private:
   std::unique_ptr< detail::rewards_api_impl > my;
};

} } } //freezone::plugins::rewards_api
