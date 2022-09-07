#ifdef IS_TEST_NET
#include <freezone/chain/generic_custom_operation_interpreter.hpp>
#include <freezone/chain/account_object.hpp>

#include <boost/test/unit_test.hpp>

#include "../db_fixture/database_fixture.hpp"

using namespace freezone::chain;
using namespace freezone::chain::test;
/*
namespace freezone { namespace plugin_tests {

using namespace freezone::app;
using namespace freezone::chain;

struct test_a_operation : base_operation
{
   string account;

   void validate() { FC_ASSERT( account.size() ); }
};

struct test_b_operation : base_operation
{
   string account;

   void validate() { FC_ASSERT( account.size() ); }
};

typedef fc::static_variant<
   test_a_operation,
   test_b_operation >   test_op;

class test_plugin : public plugin
{
   public:
      test_plugin( application* app );

      std::string plugin_name()const override { return "TEST"; }

      std::shared_ptr< generic_custom_operation_interpreter< test_op > > _evaluator_registry;
};

freezone_DEFINE_PLUGIN_EVALUATOR( test_plugin, test_a_operation, test_a );
freezone_DEFINE_PLUGIN_EVALUATOR( test_plugin, test_b_operation, test_b );

void test_a_evaluator::do_apply( const test_a_operation& o )
{
   const auto& account = db().get_account( o.account );

   db().modify( account, [&]( account_object& a )
   {
      a.json_metadata = "a";
   });
}

void test_b_evaluator::do_apply( const test_b_operation& o )
{
   const auto& account = db().get_account( o.account );

   db().modify( account, [&]( account_object& a )
   {
      a.json_metadata = "b";
   });
}

test_plugin::test_plugin( application* app ) : plugin( app )
{
   _evaluator_registry = std::make_shared< generic_custom_operation_interpreter< test_op > >( database() );

   _evaluator_registry->register_evaluator< test_a_evaluator >( *this );
   _evaluator_registry->register_evaluator< test_b_evaluator >( *this );

   database().set_custom_operation_interpreter( plugin_name(), _evaluator_registry );
}

} } // freezone::plugin_tests

freezone_DEFINE_PLUGIN( test, freezone::plugin_tests::test_plugin )

FC_REFLECT( freezone::plugin_tests::test_a_operation, (account) )
FC_REFLECT( freezone::plugin_tests::test_b_operation, (account) )

freezone_DECLARE_OPERATION_TYPE( freezone::plugin_tests::test_op );
FC_REFLECT_TYPENAME( freezone::plugin_tests::test_op );
freezone_DEFINE_OPERATION_TYPE( freezone::plugin_tests::test_op );
*/


#endif