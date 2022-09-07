
#pragma once

#include <freezone/schema/abstract_schema.hpp>
#include <freezone/schema/schema_impl.hpp>

#include <freezone/protocol/types.hpp>

namespace freezone { namespace schema { namespace detail {

//////////////////////////////////////////////
// account_name_type                        //
//////////////////////////////////////////////

struct schema_account_name_type_impl
   : public abstract_schema
{
   freezone_SCHEMA_CLASS_BODY( schema_account_name_type_impl )
};

}

template<>
struct schema_reflect< freezone::protocol::account_name_type >
{
   typedef detail::schema_account_name_type_impl           schema_impl_type;
};

} }

namespace fc {

template<>
struct get_typename< freezone::protocol::account_name_type >
{
   static const char* name()
   {
      return "freezone::protocol::account_name_type";
   }
};

}
