file(GLOB HEADERS "include/freezone/plugins/auth_util/*.hpp")

add_library( freezone_auth_util
             ${HEADERS}
             auth_util_plugin.cpp
             auth_util_api.cpp
           )

target_link_libraries( freezone_auth_util freezone_app freezone_chain freezone_protocol fc )
target_include_directories( freezone_auth_util
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )
