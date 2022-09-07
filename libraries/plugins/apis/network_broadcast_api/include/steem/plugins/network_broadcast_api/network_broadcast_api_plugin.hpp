#pragma once
#include <freezone/chain/freezone_fwd.hpp>
#include <freezone/plugins/json_rpc/json_rpc_plugin.hpp>
#include <freezone/plugins/chain/chain_plugin.hpp>
#include <freezone/plugins/p2p/p2p_plugin.hpp>
#include <freezone/plugins/rc/rc_plugin.hpp>

#include <appbase/application.hpp>

#define freezone_NETWORK_BROADCAST_API_PLUGIN_NAME "network_broadcast_api"

namespace freezone { namespace plugins { namespace network_broadcast_api {

using namespace appbase;

class network_broadcast_api_plugin : public appbase::plugin< network_broadcast_api_plugin >
{
public:
   APPBASE_PLUGIN_REQUIRES(
      (freezone::plugins::json_rpc::json_rpc_plugin)
      (freezone::plugins::rc::rc_plugin)
      (freezone::plugins::chain::chain_plugin)
      (freezone::plugins::p2p::p2p_plugin)
   )

   network_broadcast_api_plugin();
   virtual ~network_broadcast_api_plugin();

   static const std::string& name() { static std::string name = freezone_NETWORK_BROADCAST_API_PLUGIN_NAME; return name; }

   virtual void set_program_options( options_description& cli, options_description& cfg ) override;
   virtual void plugin_initialize( const variables_map& options ) override;
   virtual void plugin_startup() override;
   virtual void plugin_shutdown() override;

   std::shared_ptr< class network_broadcast_api > api;
};

} } } // freezone::plugins::network_broadcast_api
