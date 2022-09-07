file(GLOB HEADERS "include/freezone/plugins/raw_block/*.hpp")

add_library( freezone_raw_block
             ${HEADERS}
             raw_block_plugin.cpp
             raw_block_api.cpp
           )

target_link_libraries( freezone_raw_block freezone_app freezone_chain freezone_protocol fc )
target_include_directories( freezone_raw_block
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )
