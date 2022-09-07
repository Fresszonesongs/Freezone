file(GLOB HEADERS "include/freezone/blockchain_statistics/*.hpp")

add_library( freezone_blockchain_statistics
             blockchain_statistics_plugin.cpp
             blockchain_statistics_api.cpp
           )

target_link_libraries( freezone_blockchain_statistics freezone_chain freezone_protocol freezone_app )
target_include_directories( freezone_blockchain_statistics
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )

install( TARGETS
   freezone_blockchain_statistics

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)