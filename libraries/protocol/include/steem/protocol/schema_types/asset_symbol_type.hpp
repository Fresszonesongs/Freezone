
#pragma once

#include <freezone/schema/abstract_schema.hpp>
#include <freezone/schema/schema_impl.hpp>

#include <freezone/protocol/asset_symbol.hpp>

namespace freezone { namespace schema { namespace detail {

//////////////////////////////////////////////
// asset_symbol_type                        //
//////////////////////////////////////////////

struct schema_asset_symbol_type_impl
   : public abstract_schema
{
   freezone_SCHEMA_CLASS_BODY( schema_asset_symbol_type_impl )
};

}

template<>
struct schema_reflect< freezone::protocol::asset_symbol_type >
{
   typedef detail::schema_asset_symbol_type_impl           schema_impl_type;
};

} }
