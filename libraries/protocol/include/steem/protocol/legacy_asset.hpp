#pragma once

#include <freezone/protocol/asset.hpp>

#define freezone_SYMBOL_LEGACY_SER_1   (uint64_t(1) | (freezone_SYMBOL_U64 << 8))
#define freezone_SYMBOL_LEGACY_SER_2   (uint64_t(2) | (freezone_SYMBOL_U64 << 8))
#define freezone_SYMBOL_LEGACY_SER_3   (uint64_t(5) | (freezone_SYMBOL_U64 << 8))
#define freezone_SYMBOL_LEGACY_SER_4   (uint64_t(3) | (uint64_t('0') << 8) | (uint64_t('.') << 16) | (uint64_t('0') << 24) | (uint64_t('0') << 32) | (uint64_t('1') << 40))
#define freezone_SYMBOL_LEGACY_SER_5   (uint64_t(3) | (uint64_t('6') << 8) | (uint64_t('.') << 16) | (uint64_t('0') << 24) | (uint64_t('0') << 32) | (uint64_t('0') << 40))

namespace freezone { namespace protocol {

class legacy_freezone_asset_symbol_type
{
   public:
      legacy_freezone_asset_symbol_type() {}

      bool is_canon()const
      {   return ( ser == freezone_SYMBOL_SER );    }

      uint64_t ser = freezone_SYMBOL_SER;
};

struct legacy_freezone_asset
{
   public:
      legacy_freezone_asset() {}

      template< bool force_canon >
      asset to_asset()const
      {
         if( force_canon )
         {
            FC_ASSERT( symbol.is_canon(), "Must use canonical freezone symbol serialization" );
         }
         return asset( amount, freezone_SYMBOL );
      }

      static legacy_freezone_asset from_amount( share_type amount )
      {
         legacy_freezone_asset leg;
         leg.amount = amount;
         return leg;
      }

      static legacy_freezone_asset from_asset( const asset& a )
      {
         FC_ASSERT( a.symbol == freezone_SYMBOL );
         return from_amount( a.amount );
      }

      share_type                       amount;
      legacy_freezone_asset_symbol_type   symbol;
};

} }

namespace fc { namespace raw {

template< typename Stream >
inline void pack( Stream& s, const freezone::protocol::legacy_freezone_asset_symbol_type& sym )
{
   switch( sym.ser )
   {
      case freezone_SYMBOL_LEGACY_SER_1:
      case freezone_SYMBOL_LEGACY_SER_2:
      case freezone_SYMBOL_LEGACY_SER_3:
      case freezone_SYMBOL_LEGACY_SER_4:
      case freezone_SYMBOL_LEGACY_SER_5:
         wlog( "pack legacy serialization ${s}", ("s", sym.ser) );
      case freezone_SYMBOL_SER:
         pack( s, sym.ser );
         break;
      default:
         FC_ASSERT( false, "Cannot serialize legacy symbol ${s}", ("s", sym.ser) );
   }
}

template< typename Stream >
inline void unpack( Stream& s, freezone::protocol::legacy_freezone_asset_symbol_type& sym, uint32_t depth )
{
   //  994240:        "account_creation_fee": "0.1 freezone"
   // 1021529:        "account_creation_fee": "10.0 freezone"
   // 3143833:        "account_creation_fee": "3.00000 freezone"
   // 3208405:        "account_creation_fee": "2.00000 freezone"
   // 3695672:        "account_creation_fee": "3.00 freezone"
   // 4338089:        "account_creation_fee": "0.001 0.001"
   // 4626205:        "account_creation_fee": "6.000 6.000"
   // 4632595:        "account_creation_fee": "6.000 6.000"
   depth++;
   uint64_t ser = 0;

   fc::raw::unpack( s, ser, depth );
   switch( ser )
   {
      case freezone_SYMBOL_LEGACY_SER_1:
      case freezone_SYMBOL_LEGACY_SER_2:
      case freezone_SYMBOL_LEGACY_SER_3:
      case freezone_SYMBOL_LEGACY_SER_4:
      case freezone_SYMBOL_LEGACY_SER_5:
         wlog( "unpack legacy serialization ${s}", ("s", ser) );
      case freezone_SYMBOL_SER:
         sym.ser = ser;
         break;
      default:
         FC_ASSERT( false, "Cannot deserialize legacy symbol ${s}", ("s", ser) );
   }
}

} // fc::raw

inline void to_variant( const freezone::protocol::legacy_freezone_asset& leg, fc::variant& v )
{
   to_variant( leg.to_asset<false>(), v );
}

inline void from_variant( const fc::variant& v, freezone::protocol::legacy_freezone_asset& leg )
{
   freezone::protocol::asset a;
   from_variant( v, a );
   leg = freezone::protocol::legacy_freezone_asset::from_asset( a );
}

template<>
struct get_typename< freezone::protocol::legacy_freezone_asset_symbol_type >
{
   static const char* name()
   {
      return "freezone::protocol::legacy_freezone_asset_symbol_type";
   }
};

} // fc

FC_REFLECT( freezone::protocol::legacy_freezone_asset,
   (amount)
   (symbol)
   )
