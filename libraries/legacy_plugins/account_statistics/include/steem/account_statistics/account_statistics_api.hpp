#pragma once

#include <freezone/account_statistics/account_statistics_plugin.hpp>

#include <fc/api.hpp>

namespace freezone { namespace app {
   struct api_context;
} }

namespace freezone { namespace account_statistics {

namespace detail
{
   class account_statistics_api_impl;
}

class account_statistics_api
{
   public:
      account_statistics_api( const freezone::app::api_context& ctx );

      void on_api_startup();

   private:
      std::shared_ptr< detail::account_statistics_api_impl > _my;
};

} } // freezone::account_statistics

FC_API( freezone::account_statistics::account_statistics_api, )