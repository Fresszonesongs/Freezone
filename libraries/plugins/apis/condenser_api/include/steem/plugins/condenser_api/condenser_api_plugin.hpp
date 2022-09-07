#pragma once
#include <freezone/chain/freezone_fwd.hpp>

#include <freezone/plugins/json_rpc/json_rpc_plugin.hpp>
#include <freezone/plugins/database_api/database_api_plugin.hpp>
#include <freezone/plugins/block_api/block_api_plugin.hpp>

#define freezone_CONDENSER_API_PLUGIN_NAME "condenser_api"

namespace freezone { namespace plugins { namespace condenser_api {

using namespace appbase;

class condenser_api_plugin : public appbase::plugin< condenser_api_plugin >
{
public:
   APPBASE_PLUGIN_REQUIRES( (freezone::plugins::json_rpc::json_rpc_plugin)(freezone::plugins::database_api::database_api_plugin) )

   condenser_api_plugin();
   virtual ~condenser_api_plugin();

   static const std::string& name() { static std::string name = freezone_CONDENSER_API_PLUGIN_NAME; return name; }

   virtual void set_program_options( options_description& cli, options_description& cfg ) override;

   virtual void plugin_initialize( const variables_map& options ) override;
   virtual void plugin_startup() override;
   virtual void plugin_shutdown() override;

   std::shared_ptr< class condenser_api > api;
};

} } } // freezone::plugins::condenser_api
