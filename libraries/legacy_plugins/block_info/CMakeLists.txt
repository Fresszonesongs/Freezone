file(GLOB HEADERS "include/freezone/plugins/block_info/*.hpp")

add_library( freezone_block_info
             ${HEADERS}
             block_info_plugin.cpp
             block_info_api.cpp
           )

target_link_libraries( freezone_block_info freezone_app freezone_chain freezone_protocol fc )
target_include_directories( freezone_block_info
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )
