#pragma once
#include <freezone/chain/database.hpp>
#include <freezone/protocol/asset_symbol.hpp>

namespace freezone { namespace chain {

   void replenish_nai_pool( database& db );
   void remove_from_nai_pool( database &db, const asset_symbol_type& a );

} } // freezone::chain
