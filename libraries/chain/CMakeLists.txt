file(GLOB HEADERS "include/freezone/chain/*.hpp" "include/freezone/chain/util/*.hpp" "include/freezone/chain/SST_objects/*.hpp" "include/freezone/chain/sps_objects/*.hpp")

## SORT .cpp by most likely to change / break compile
add_library( freezone_chain

             # As database takes the longest to compile, start it first
             database.cpp
             index.cpp

             SST_evaluator.cpp
             sps_evaluator.cpp

             SST_objects/SST_market_maker.cpp
             SST_objects/nai_pool.cpp
             freezone_evaluator.cpp
             freezone_objects.cpp
             required_action_evaluator.cpp
             optional_action_evaluator.cpp

             witness_schedule.cpp
             fork_database.cpp
             comment_rewards.cpp

             shared_authority.cpp
             block_log.cpp

             generic_custom_operation_interpreter.cpp

             util/rd.cpp
             util/reward.cpp
             util/impacted.cpp
             util/advanced_benchmark_dumper.cpp
             util/SST_token.cpp
             util/sps_processor.cpp
             util/sps_helper.cpp

             ${HEADERS}
           )

target_link_libraries( freezone_chain freezone_jsonball freezone_protocol fc chainbase freezone_schema appbase mira
                       ${PATCH_MERGE_LIB} )
target_include_directories( freezone_chain
                            PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}/include" )

if( CLANG_TIDY_EXE )
   set_target_properties(
      freezone_chain PROPERTIES
      CXX_CLANG_TIDY "${DO_CLANG_TIDY}"
   )
endif( CLANG_TIDY_EXE )

if(MSVC)
  set_source_files_properties( database.cpp PROPERTIES COMPILE_FLAGS "/bigobj" )
endif(MSVC)

INSTALL( TARGETS
   freezone_chain

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)
INSTALL( FILES ${HEADERS} DESTINATION "include/freezone/chain" )
