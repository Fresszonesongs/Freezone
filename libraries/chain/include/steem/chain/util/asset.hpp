#pragma once

#include <freezone/protocol/asset.hpp>

namespace freezone { namespace chain { namespace util {

using freezone::protocol::asset;
using freezone::protocol::price;

inline asset to_sbd( const price& p, const asset& freezone )
{
   FC_ASSERT( freezone.symbol == freezone_SYMBOL );
   if( p.is_null() )
      return asset( 0, SBD_SYMBOL );
   return freezone * p;
}

inline asset to_freezone( const price& p, const asset& sbd )
{
   FC_ASSERT( sbd.symbol == SBD_SYMBOL );
   if( p.is_null() )
      return asset( 0, freezone_SYMBOL );
   return sbd * p;
}

} } }
