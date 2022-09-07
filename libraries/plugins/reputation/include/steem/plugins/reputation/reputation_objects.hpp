#pragma once
#include <freezone/chain/freezone_object_types.hpp>

namespace freezone { namespace chain {
struct by_account;
} }

namespace freezone { namespace plugins { namespace reputation {

using namespace std;
using namespace freezone::chain;

using chainbase::t_vector;

#ifndef freezone_REPUTATION_SPACE_ID
#define freezone_REPUTATION_SPACE_ID 17
#endif

enum reputation_plugin_object_type
{
   reputation_object_type        = ( freezone_REPUTATION_SPACE_ID << 8 )
};


class reputation_object : public object< reputation_object_type, reputation_object >
{
   public:
      template< typename Constructor, typename Allocator >
      reputation_object( Constructor&& c, allocator< Allocator > a )
      {
         c( *this );
      }

      reputation_object() {}

      id_type           id;

      account_name_type account;
      share_type        reputation;
};

typedef oid< reputation_object > reputation_id_type;

typedef multi_index_container<
   reputation_object,
   indexed_by<
      ordered_unique< tag< by_id >, member< reputation_object, reputation_id_type, &reputation_object::id > >,
      ordered_unique< tag< by_account >, member< reputation_object, account_name_type, &reputation_object::account > >
   >,
   allocator< reputation_object >
> reputation_index;

} } } // freezone::plugins::reputation


FC_REFLECT( freezone::plugins::reputation::reputation_object, (id)(account)(reputation) )
CHAINBASE_SET_INDEX_TYPE( freezone::plugins::reputation::reputation_object, freezone::plugins::reputation::reputation_index )
