#pragma once
#include <freezone/protocol/types.hpp>
#include <freezone/protocol/base.hpp>
#include <freezone/protocol/asset.hpp>
#include <freezone/protocol/misc_utilities.hpp>

namespace freezone { namespace protocol {

#ifdef IS_TEST_NET
   struct example_required_action : public base_operation
   {
      account_name_type account;

      void validate()const;
      void get_required_active_authorities( flat_set<account_name_type>& a )const{ a.insert(account); }

      friend bool operator==( const example_required_action& lhs, const example_required_action& rhs );
   };
#endif

   struct SST_refund_action : public base_operation
   {
      account_name_type contributor;
      asset_symbol_type symbol;
      uint32_t          contribution_id;
      asset             refund;

      void validate()const;
      void get_required_active_authorities( flat_set<account_name_type>& a )const { a.insert( contributor ); }

      friend bool operator==( const SST_refund_action& lhs, const SST_refund_action& rhs );
   };

   struct contribution_payout
   {
      asset payout;
      bool  to_vesting;

      friend bool operator==( const contribution_payout& rhs, const contribution_payout& lhs );
   };

   struct SST_contributor_payout_action : public base_operation
   {
      account_name_type    contributor;
      asset_symbol_type    symbol;
      uint32_t             contribution_id;
      asset                contribution;
      std::vector< contribution_payout > payouts;

      void validate()const;
      void get_required_active_authorities( flat_set<account_name_type>& a )const { a.insert( contributor ); }

      friend bool operator==( const SST_contributor_payout_action& lhs, const SST_contributor_payout_action& rhs );
   };

   struct SST_founder_payout_action : public base_operation
   {
      asset_symbol_type                                   symbol;
      std::map< account_name_type, std::vector< contribution_payout > > account_payouts;
      share_type                                          market_maker_freezone  = 0;
      share_type                                          market_maker_tokens = 0;
      share_type                                          reward_balance      = 0;

      void validate()const;
      void get_required_active_authorities( flat_set<account_name_type>& a )const
      {
         for ( auto& entry : account_payouts )
            a.insert( entry.first );
      }

      friend bool operator==( const SST_founder_payout_action& lhs, const SST_founder_payout_action& rhs );
   };

   struct SST_ico_launch_action : public base_operation
   {
      account_name_type control_account;
      asset_symbol_type symbol;

      void validate()const;
      void get_required_active_authorities( flat_set<account_name_type>& a )const { a.insert( control_account ); }

      friend bool operator==( const SST_ico_launch_action& lhs, const SST_ico_launch_action& rhs );
   };

   struct SST_ico_evaluation_action : public base_operation
   {
      account_name_type control_account;
      asset_symbol_type symbol;

      void validate()const;
      void get_required_active_authorities( flat_set<account_name_type>& a )const { a.insert( control_account ); }

      friend bool operator==( const SST_ico_evaluation_action& lhs, const SST_ico_evaluation_action& rhs );
   };

   struct SST_token_launch_action : public base_operation
   {
      account_name_type control_account;
      asset_symbol_type symbol;

      void validate()const;
      void get_required_active_authorities( flat_set<account_name_type>& a )const { a.insert( control_account ); }

      friend bool operator==( const SST_token_launch_action& lhs, const SST_token_launch_action& rhs );
   };
} } // freezone::protocol

#ifdef IS_TEST_NET
FC_REFLECT( freezone::protocol::example_required_action, (account) )
#endif

FC_REFLECT( freezone::protocol::SST_refund_action, (contributor)(symbol)(contribution_id)(refund) )
FC_REFLECT( freezone::protocol::contribution_payout, (payout)(to_vesting) )
FC_REFLECT( freezone::protocol::SST_contributor_payout_action, (contributor)(symbol)(contribution_id)(contribution)(payouts) )
FC_REFLECT( freezone::protocol::SST_founder_payout_action, (symbol)(account_payouts)(market_maker_freezone)(market_maker_tokens)(reward_balance) )
FC_REFLECT( freezone::protocol::SST_ico_launch_action, (control_account)(symbol) )
FC_REFLECT( freezone::protocol::SST_ico_evaluation_action, (control_account)(symbol) )
FC_REFLECT( freezone::protocol::SST_token_launch_action, (control_account)(symbol) )
