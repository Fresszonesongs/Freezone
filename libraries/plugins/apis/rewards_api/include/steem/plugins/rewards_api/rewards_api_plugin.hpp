#pragma once
#include <freezone/chain/freezone_fwd.hpp>
#include <freezone/plugins/rewards_api/rewards_api.hpp>
#include <freezone/plugins/json_rpc/json_rpc_plugin.hpp>

#include <appbase/application.hpp>

namespace freezone { namespace plugins { namespace rewards_api {

#define freezone_REWARDS_API_PLUGIN_NAME "rewards_api"

class rewards_api_plugin : public appbase::plugin< rewards_api_plugin >
{
   public:
      rewards_api_plugin();
      virtual ~rewards_api_plugin();

      APPBASE_PLUGIN_REQUIRES(
         (freezone::plugins::json_rpc::json_rpc_plugin)
      )

      static const std::string& name() { static std::string name = freezone_REWARDS_API_PLUGIN_NAME; return name; }

      virtual void set_program_options(
         boost::program_options::options_description& cli,
         boost::program_options::options_description& cfg ) override;
      virtual void plugin_initialize( const boost::program_options::variables_map& options ) override;
      virtual void plugin_startup() override;
      virtual void plugin_shutdown() override;

   private:
      std::unique_ptr< rewards_api > api;
};

} } } // freezone::plugins::rewards_api

