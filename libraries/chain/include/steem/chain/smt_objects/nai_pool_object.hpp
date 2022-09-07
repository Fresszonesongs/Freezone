#pragma once

#include <freezone/chain/freezone_fwd.hpp>
#include <freezone/chain/freezone_object_types.hpp>
#include <freezone/protocol/asset_symbol.hpp>

namespace freezone { namespace chain {

   class nai_pool_object : public object< nai_pool_object_type, nai_pool_object >
   {
      freezone_STD_ALLOCATOR_CONSTRUCTOR( nai_pool_object );

   public:
      using pool_type = fc::array< asset_symbol_type, SST_MAX_NAI_POOL_COUNT >;

      template< typename Constructor, typename Allocator >
      nai_pool_object( Constructor&& c, allocator< Allocator > a )
      {
         c( *this );
      }

      id_type id;

      uint8_t num_available_nais    = 0;
      uint32_t attempts_per_block   = 0;
      uint32_t collisions_per_block = 0;
      block_id_type last_block_id   = block_id_type();

      pool_type nais;

      std::vector< asset_symbol_type > pool() const
      {
         return std::vector< asset_symbol_type >{ nais.begin(), nais.end() - (SST_MAX_NAI_POOL_COUNT - num_available_nais) };
      }

      bool contains( const asset_symbol_type& a ) const
      {
         const auto end = nais.end() - (SST_MAX_NAI_POOL_COUNT - num_available_nais);
         return std::find( nais.begin(), end, asset_symbol_type::from_asset_num( a.get_stripped_precision_SST_num() ) ) != end;
      }
   };

   typedef multi_index_container <
      nai_pool_object,
      indexed_by<
         ordered_unique< tag< by_id >, member< nai_pool_object, nai_pool_id_type, &nai_pool_object::id > >
      >,
      allocator< nai_pool_object >
   > nai_pool_index;

} } // namespace freezone::chain

FC_REFLECT( freezone::chain::nai_pool_object, (id)(num_available_nais)(attempts_per_block)(collisions_per_block)(last_block_id)(nais) )

CHAINBASE_SET_INDEX_TYPE( freezone::chain::nai_pool_object, freezone::chain::nai_pool_index )
