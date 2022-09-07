#pragma once
#include <freezone/chain/freezone_fwd.hpp>
#include <freezone/plugins/market_history/market_history_plugin.hpp>
#include <freezone/plugins/json_rpc/json_rpc_plugin.hpp>

#include <appbase/application.hpp>

#define freezone_MARKET_HISTORY_API_PLUGIN_NAME "market_history_api"


namespace freezone { namespace plugins { namespace market_history {

using namespace appbase;

class market_history_api_plugin : public appbase::plugin< market_history_api_plugin >
{
public:
   APPBASE_PLUGIN_REQUIRES(
      (freezone::plugins::market_history::market_history_plugin)
      (freezone::plugins::json_rpc::json_rpc_plugin)
   )

   market_history_api_plugin();
   virtual ~market_history_api_plugin();

   static const std::string& name() { static std::string name = freezone_MARKET_HISTORY_API_PLUGIN_NAME; return name; }

   virtual void set_program_options( options_description& cli, options_description& cfg ) override;

   virtual void plugin_initialize( const variables_map& options ) override;
   virtual void plugin_startup() override;
   virtual void plugin_shutdown() override;

   std::shared_ptr< class market_history_api > api;
};

} } } // freezone::plugins::market_history
