#include <freezone/plugins/follow/follow_operations.hpp>

#include <freezone/protocol/operation_util_impl.hpp>

namespace freezone { namespace plugins{ namespace follow {

void follow_operation::validate()const
{
   FC_ASSERT( follower != following, "You cannot follow yourself" );
}

void reblog_operation::validate()const
{
   FC_ASSERT( account != author, "You cannot reblog your own content" );
}

} } } //freezone::plugins::follow

freezone_DEFINE_OPERATION_TYPE( freezone::plugins::follow::follow_plugin_operation )
