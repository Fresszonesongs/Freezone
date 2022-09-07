#ifdef IS_TEST_NET
#include <boost/test/unit_test.hpp>

#include <freezone/chain/account_object.hpp>
#include <freezone/chain/comment_object.hpp>
#include <freezone/protocol/freezone_operations.hpp>

#include <freezone/plugins/market_history/market_history_plugin.hpp>

#include "../db_fixture/database_fixture.hpp"

using namespace freezone::chain;
using namespace freezone::protocol;

BOOST_FIXTURE_TEST_SUITE( market_history, database_fixture )

BOOST_AUTO_TEST_CASE( mh_test )
{
   using namespace freezone::plugins::market_history;

   try
   {
      int argc = boost::unit_test::framework::master_test_suite().argc;
      char** argv = boost::unit_test::framework::master_test_suite().argv;
      for( int i=1; i<argc; i++ )
      {
         const std::string arg = argv[i];
         if( arg == "--record-assert-trip" )
            fc::enable_record_assert_trip = true;
         if( arg == "--show-test-names" )
            std::cout << "running test " << boost::unit_test::framework::current_test_case().p_name << std::endl;
      }

      appbase::app().register_plugin< market_history_plugin >();
      db_plugin = &appbase::app().register_plugin< freezone::plugins::debug_node::debug_node_plugin >();
      init_account_pub_key = init_account_priv_key.get_public_key();

      db_plugin->logging = false;
      appbase::app().initialize<
         freezone::plugins::market_history::market_history_plugin,
         freezone::plugins::debug_node::debug_node_plugin
      >( argc, argv );

      db = &appbase::app().get_plugin< freezone::plugins::chain::chain_plugin >().db();
      BOOST_REQUIRE( db );

      open_database();

      generate_block();
      db->set_hardfork( freezone_NUM_HARDFORKS );
      generate_block();

      vest( "initminer", 10000 );

      // Fill up the rest of the required miners
      for( int i = freezone_NUM_INIT_MINERS; i < freezone_MAX_WITNESSES; i++ )
      {
         account_create( freezone_INIT_MINER_NAME + fc::to_string( i ), init_account_pub_key );
         fund( freezone_INIT_MINER_NAME + fc::to_string( i ), freezone_MIN_PRODUCER_REWARD.amount.value );
         witness_create( freezone_INIT_MINER_NAME + fc::to_string( i ), init_account_priv_key, "foo.bar", init_account_pub_key, freezone_MIN_PRODUCER_REWARD.amount );
      }

      validate_database();

      ACTORS( (alice)(bob)(sam) );
      generate_block();

      fund( "alice", ASSET( "1000.000 TESTS" ) );
      fund( "alice", ASSET( "1000.000 TBD" ) );
      fund( "bob", ASSET( "1000.000 TESTS" ) );
      fund( "sam", ASSET( "1000.000 TESTS" ) );

      const auto& bucket_idx = db->get_index< bucket_index, by_bucket >();
      const auto& order_hist_idx = db->get_index< order_history_index, by_id >();

      BOOST_REQUIRE( bucket_idx.begin() == bucket_idx.end() );
      BOOST_REQUIRE( order_hist_idx.begin() == order_hist_idx.end() );
      validate_database();

      signed_transaction tx;

      auto fill_order_a_time = db->head_block_time();
      auto time_a = fc::time_point_sec( ( fill_order_a_time.sec_since_epoch() / 15 ) * 15 );

      limit_order_create2_operation op;
      op.owner = "alice";
      op.amount_to_sell = ASSET( "1.000 TBD" );
      op.exchange_rate  = price( ASSET( "0.500 TBD" ), ASSET( "1.000 TESTS" ) );
      op.expiration = db->head_block_time() + fc::seconds( freezone_MAX_LIMIT_ORDER_EXPIRATION );
      tx.operations.push_back( op );
      tx.set_expiration( db->head_block_time() + freezone_MAX_TIME_UNTIL_EXPIRATION );
      sign( tx, alice_private_key );
      db->push_transaction( tx,  0 );

      tx.operations.clear();
      tx.signatures.clear();

      op.owner = "bob";
      op.amount_to_sell = ASSET( "1.500 TESTS" );
      op.exchange_rate  = price( ASSET( "0.500 TBD" ), ASSET( "1.000 TESTS" ) );
      tx.operations.push_back( op );
      sign(tx, bob_private_key);
      db->push_transaction( tx, 0 );

      generate_blocks( db->head_block_time() + ( 60 * 90 ) );

      auto fill_order_b_time = db->head_block_time();

      tx.operations.clear();
      tx.signatures.clear();

      op.owner = "sam";
      op.amount_to_sell = ASSET( "1.000 TESTS" );
      op.exchange_rate  = price( ASSET( "0.500 TBD" ), ASSET( "1.000 TESTS" ) );
      tx.operations.push_back( op );
      tx.set_expiration( db->head_block_time() + freezone_MAX_TIME_UNTIL_EXPIRATION );
      sign( tx, sam_private_key );
      db->push_transaction( tx, 0 );

      generate_blocks( db->head_block_time() + 60 );

      auto fill_order_c_time = db->head_block_time();

      tx.operations.clear();
      tx.signatures.clear();

      op.owner = "alice";
      op.amount_to_sell = ASSET( "0.500 TBD" );
      op.exchange_rate  = price( ASSET( "0.556 TBD" ), ASSET( "1.000 TESTS" ) );
      tx.operations.push_back( op );
      tx.set_expiration( db->head_block_time() + freezone_MAX_TIME_UNTIL_EXPIRATION );
      sign( tx, alice_private_key );
      db->push_transaction( tx, 0 );

      tx.operations.clear();
      tx.signatures.clear();

      op.owner = "bob";
      op.amount_to_sell = ASSET( "0.450 TESTS" );
      op.exchange_rate  = price( ASSET( "0.556 TBD" ), ASSET( "1.000 TESTS" ) );
      tx.operations.push_back( op );
      tx.set_expiration( db->head_block_time() + freezone_MAX_TIME_UNTIL_EXPIRATION );
      sign( tx, bob_private_key );
      db->push_transaction( tx, 0 );
      validate_database();

      auto bucket = bucket_idx.begin();

      BOOST_REQUIRE( bucket->seconds == 15 );
      BOOST_REQUIRE( bucket->open == time_a );
      BOOST_REQUIRE( bucket->freezone.high == ASSET( "1.500 TESTS " ).amount );
      BOOST_REQUIRE( bucket->freezone.low == ASSET( "1.500 TESTS" ).amount );
      BOOST_REQUIRE( bucket->freezone.open == ASSET( "1.500 TESTS" ).amount );
      BOOST_REQUIRE( bucket->freezone.close == ASSET( "1.500 TESTS").amount );
      BOOST_REQUIRE( bucket->freezone.volume == ASSET( "1.500 TESTS" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.high == ASSET( "0.750 TBD" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.low == ASSET( "0.750 TBD" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.open == ASSET( "0.750 TBD" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.close == ASSET( "0.750 TBD" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.volume == ASSET( "0.750 TBD" ).amount );

      bucket++;

      BOOST_REQUIRE( bucket->seconds == 15 );
      BOOST_REQUIRE( bucket->open == time_a + ( 60 * 90 ) );
      BOOST_REQUIRE( bucket->freezone.high == ASSET( "0.500 TESTS " ).amount );
      BOOST_REQUIRE( bucket->freezone.low == ASSET( "0.500 TESTS" ).amount );
      BOOST_REQUIRE( bucket->freezone.open == ASSET( "0.500 TESTS" ).amount );
      BOOST_REQUIRE( bucket->freezone.close == ASSET( "0.500 TESTS").amount );
      BOOST_REQUIRE( bucket->freezone.volume == ASSET( "0.500 TESTS" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.high == ASSET( "0.250 TBD" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.low == ASSET( "0.250 TBD" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.open == ASSET( "0.250 TBD" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.close == ASSET( "0.250 TBD" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.volume == ASSET( "0.250 TBD" ).amount );

      bucket++;

      BOOST_REQUIRE( bucket->seconds == 15 );
      BOOST_REQUIRE( bucket->open == time_a + ( 60 * 90 ) + 60 );
      BOOST_REQUIRE( bucket->freezone.high == ASSET( "0.449 TESTS " ).amount );
      BOOST_REQUIRE( bucket->freezone.low == ASSET( "0.500 TESTS" ).amount );
      BOOST_REQUIRE( bucket->freezone.open == ASSET( "0.500 TESTS" ).amount );
      BOOST_REQUIRE( bucket->freezone.close == ASSET( "0.449 TESTS").amount );
      BOOST_REQUIRE( bucket->freezone.volume == ASSET( "0.949 TESTS" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.high == ASSET( "0.250 TBD" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.low == ASSET( "0.250 TBD" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.open == ASSET( "0.250 TBD" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.close == ASSET( "0.250 TBD" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.volume == ASSET( "0.500 TBD" ).amount );
      bucket++;

      BOOST_REQUIRE( bucket->seconds == 60 );
      BOOST_REQUIRE( bucket->open == time_a );
      BOOST_REQUIRE( bucket->freezone.high == ASSET( "1.500 TESTS " ).amount );
      BOOST_REQUIRE( bucket->freezone.low == ASSET( "1.500 TESTS" ).amount );
      BOOST_REQUIRE( bucket->freezone.open == ASSET( "1.500 TESTS" ).amount );
      BOOST_REQUIRE( bucket->freezone.close == ASSET( "1.500 TESTS").amount );
      BOOST_REQUIRE( bucket->freezone.volume == ASSET( "1.500 TESTS" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.high == ASSET( "0.750 TBD" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.low == ASSET( "0.750 TBD" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.open == ASSET( "0.750 TBD" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.close == ASSET( "0.750 TBD" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.volume == ASSET( "0.750 TBD" ).amount );
      bucket++;

      BOOST_REQUIRE( bucket->seconds == 60 );
      BOOST_REQUIRE( bucket->open == time_a + ( 60 * 90 ) );
      BOOST_REQUIRE( bucket->freezone.high == ASSET( "0.500 TESTS " ).amount );
      BOOST_REQUIRE( bucket->freezone.low == ASSET( "0.500 TESTS" ).amount );
      BOOST_REQUIRE( bucket->freezone.open == ASSET( "0.500 TESTS" ).amount );
      BOOST_REQUIRE( bucket->freezone.close == ASSET( "0.500 TESTS").amount );
      BOOST_REQUIRE( bucket->freezone.volume == ASSET( "0.500 TESTS" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.high == ASSET( "0.250 TBD" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.low == ASSET( "0.250 TBD" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.open == ASSET( "0.250 TBD" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.close == ASSET( "0.250 TBD" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.volume == ASSET( "0.250 TBD" ).amount );
      bucket++;

      BOOST_REQUIRE( bucket->seconds == 60 );
      BOOST_REQUIRE( bucket->open == time_a + ( 60 * 90 ) + 60 );
      BOOST_REQUIRE( bucket->freezone.high == ASSET( "0.449 TESTS " ).amount );
      BOOST_REQUIRE( bucket->freezone.low == ASSET( "0.500 TESTS" ).amount );
      BOOST_REQUIRE( bucket->freezone.open == ASSET( "0.500 TESTS" ).amount );
      BOOST_REQUIRE( bucket->freezone.close == ASSET( "0.449 TESTS").amount );
      BOOST_REQUIRE( bucket->freezone.volume == ASSET( "0.949 TESTS" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.high == ASSET( "0.250 TBD" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.low == ASSET( "0.250 TBD" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.open == ASSET( "0.250 TBD" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.close == ASSET( "0.250 TBD" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.volume == ASSET( "0.500 TBD" ).amount );
      bucket++;

      BOOST_REQUIRE( bucket->seconds == 300 );
      BOOST_REQUIRE( bucket->open == time_a );
      BOOST_REQUIRE( bucket->freezone.high == ASSET( "1.500 TESTS " ).amount );
      BOOST_REQUIRE( bucket->freezone.low == ASSET( "1.500 TESTS" ).amount );
      BOOST_REQUIRE( bucket->freezone.open == ASSET( "1.500 TESTS" ).amount );
      BOOST_REQUIRE( bucket->freezone.close == ASSET( "1.500 TESTS").amount );
      BOOST_REQUIRE( bucket->freezone.volume == ASSET( "1.500 TESTS" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.high == ASSET( "0.750 TBD" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.low == ASSET( "0.750 TBD" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.open == ASSET( "0.750 TBD" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.close == ASSET( "0.750 TBD" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.volume == ASSET( "0.750 TBD" ).amount );
      bucket++;

      BOOST_REQUIRE( bucket->seconds == 300 );
      BOOST_REQUIRE( bucket->open == time_a + ( 60 * 90 ) );
      BOOST_REQUIRE( bucket->freezone.high == ASSET( "0.449 TESTS " ).amount );
      BOOST_REQUIRE( bucket->freezone.low == ASSET( "0.500 TESTS" ).amount );
      BOOST_REQUIRE( bucket->freezone.open == ASSET( "0.500 TESTS" ).amount );
      BOOST_REQUIRE( bucket->freezone.close == ASSET( "0.449 TESTS").amount );
      BOOST_REQUIRE( bucket->freezone.volume == ASSET( "1.449 TESTS" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.high == ASSET( "0.250 TBD" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.low == ASSET( "0.250 TBD" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.open == ASSET( "0.250 TBD" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.close == ASSET( "0.250 TBD" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.volume == ASSET( "0.750 TBD" ).amount );
      bucket++;

      BOOST_REQUIRE( bucket->seconds == 3600 );
      BOOST_REQUIRE( bucket->open == time_a );
      BOOST_REQUIRE( bucket->freezone.high == ASSET( "1.500 TESTS " ).amount );
      BOOST_REQUIRE( bucket->freezone.low == ASSET( "1.500 TESTS" ).amount );
      BOOST_REQUIRE( bucket->freezone.open == ASSET( "1.500 TESTS" ).amount );
      BOOST_REQUIRE( bucket->freezone.close == ASSET( "1.500 TESTS").amount );
      BOOST_REQUIRE( bucket->freezone.volume == ASSET( "1.500 TESTS" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.high == ASSET( "0.750 TBD" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.low == ASSET( "0.750 TBD" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.open == ASSET( "0.750 TBD" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.close == ASSET( "0.750 TBD" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.volume == ASSET( "0.750 TBD" ).amount );
      bucket++;

      BOOST_REQUIRE( bucket->seconds == 3600 );
      BOOST_REQUIRE( bucket->open == time_a + ( 60 * 60 ) );
      BOOST_REQUIRE( bucket->freezone.high == ASSET( "0.449 TESTS " ).amount );
      BOOST_REQUIRE( bucket->freezone.low == ASSET( "0.500 TESTS" ).amount );
      BOOST_REQUIRE( bucket->freezone.open == ASSET( "0.500 TESTS" ).amount );
      BOOST_REQUIRE( bucket->freezone.close == ASSET( "0.449 TESTS").amount );
      BOOST_REQUIRE( bucket->freezone.volume == ASSET( "1.449 TESTS" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.high == ASSET( "0.250 TBD" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.low == ASSET( "0.250 TBD" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.open == ASSET( "0.250 TBD" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.close == ASSET( "0.250 TBD" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.volume == ASSET( "0.750 TBD" ).amount );
      bucket++;

      BOOST_REQUIRE( bucket->seconds == 21600 );
      BOOST_REQUIRE( bucket->open == freezone_GENESIS_TIME );
      BOOST_REQUIRE( bucket->freezone.high == ASSET( "0.449 TESTS " ).amount );
      BOOST_REQUIRE( bucket->freezone.low == ASSET( "1.500 TESTS" ).amount );
      BOOST_REQUIRE( bucket->freezone.open == ASSET( "1.500 TESTS" ).amount );
      BOOST_REQUIRE( bucket->freezone.close == ASSET( "0.449 TESTS").amount );
      BOOST_REQUIRE( bucket->freezone.volume == ASSET( "2.949 TESTS" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.high == ASSET( "0.250 TBD" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.low == ASSET( "0.750 TBD" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.open == ASSET( "0.750 TBD" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.close == ASSET( "0.250 TBD" ).amount );
      BOOST_REQUIRE( bucket->non_freezone.volume == ASSET( "1.500 TBD" ).amount );
      bucket++;

      BOOST_REQUIRE( bucket == bucket_idx.end() );

      auto order = order_hist_idx.begin();

      BOOST_REQUIRE( order->time == fill_order_a_time );
      BOOST_REQUIRE( order->op.current_owner == "bob" );
      BOOST_REQUIRE( order->op.current_orderid == 0 );
      BOOST_REQUIRE( order->op.current_pays == ASSET( "1.500 TESTS" ) );
      BOOST_REQUIRE( order->op.open_owner == "alice" );
      BOOST_REQUIRE( order->op.open_orderid == 0 );
      BOOST_REQUIRE( order->op.open_pays == ASSET( "0.750 TBD" ) );
      order++;

      BOOST_REQUIRE( order->time == fill_order_b_time );
      BOOST_REQUIRE( order->op.current_owner == "sam" );
      BOOST_REQUIRE( order->op.current_orderid == 0 );
      BOOST_REQUIRE( order->op.current_pays == ASSET( "0.500 TESTS" ) );
      BOOST_REQUIRE( order->op.open_owner == "alice" );
      BOOST_REQUIRE( order->op.open_orderid == 0 );
      BOOST_REQUIRE( order->op.open_pays == ASSET( "0.250 TBD" ) );
      order++;

      BOOST_REQUIRE( order->time == fill_order_c_time );
      BOOST_REQUIRE( order->op.current_owner == "alice" );
      BOOST_REQUIRE( order->op.current_orderid == 0 );
      BOOST_REQUIRE( order->op.current_pays == ASSET( "0.250 TBD" ) );
      BOOST_REQUIRE( order->op.open_owner == "sam" );
      BOOST_REQUIRE( order->op.open_orderid == 0 );
      BOOST_REQUIRE( order->op.open_pays == ASSET( "0.500 TESTS" ) );
      order++;

      BOOST_REQUIRE( order->time == fill_order_c_time );
      BOOST_REQUIRE( order->op.current_owner == "bob" );
      BOOST_REQUIRE( order->op.current_orderid == 0 );
      BOOST_REQUIRE( order->op.current_pays == ASSET( "0.449 TESTS" ) );
      BOOST_REQUIRE( order->op.open_owner == "alice" );
      BOOST_REQUIRE( order->op.open_orderid == 0 );
      BOOST_REQUIRE( order->op.open_pays == ASSET( "0.250 TBD" ) );
      order++;

      BOOST_REQUIRE( order == order_hist_idx.end() );
   }
   FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_SUITE_END()
#endif
