file(GLOB HEADERS "include/freezone/plugins/rewards_api/*.hpp")

add_library( rewards_api_plugin
             ${HEADERS}
             rewards_api.cpp
             rewards_api_plugin.cpp
           )

target_link_libraries( rewards_api_plugin appbase freezone_chain fc json_rpc_plugin )
target_include_directories( rewards_api_plugin
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )
