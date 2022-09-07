file(GLOB HEADERS "include/freezone/account_statistics/*.hpp")

add_library( freezone_account_statistics
             account_statistics_plugin.cpp
             account_statistics_api.cpp
           )

target_link_libraries( freezone_account_statistics freezone_chain freezone_protocol freezone_app )
target_include_directories( freezone_account_statistics
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )

install( TARGETS
   freezone_account_statistics

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)