#pragma once
#include <freezone/protocol/types.hpp>
#include <freezone/protocol/base.hpp>
#include <freezone/protocol/asset.hpp>
#include <freezone/protocol/SST_operations.hpp>

namespace freezone { namespace protocol {

#ifdef IS_TEST_NET
   struct example_optional_action : public base_operation
   {
      account_name_type account;

      void validate()const;
      void get_required_active_authorities( flat_set<account_name_type>& a )const{ a.insert(account); }
   };
#endif

   struct SST_token_emission_action : public base_operation
   {
      account_name_type                        control_account;
      asset_symbol_type                        symbol;
      time_point_sec                           emission_time;
      flat_map< unit_target_type, share_type > emissions;

      extensions_type                          extensions;

      void validate()const;
      void get_required_active_authorities( flat_set<account_name_type>& a )const{ a.insert(control_account); }
   };

} } // freezone::protocol

#ifdef IS_TEST_NET
FC_REFLECT( freezone::protocol::example_optional_action, (account) )
#endif

FC_REFLECT( freezone::protocol::SST_token_emission_action, (control_account)(symbol)(emission_time)(emissions)(extensions) )
