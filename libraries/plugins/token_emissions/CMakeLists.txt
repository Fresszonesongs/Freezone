file(GLOB HEADERS "include/freezone/plugins/token_emissions/*.hpp")

add_library( token_emissions_plugin
             token_emissions_plugin.cpp
             ${HEADERS}
           )

target_link_libraries( token_emissions_plugin chain_plugin )
target_include_directories( token_emissions_plugin
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )

if( CLANG_TIDY_EXE )
   set_target_properties(
      transaction_status_plugin PROPERTIES
      CXX_CLANG_TIDY "${DO_CLANG_TIDY}"
   )
endif( CLANG_TIDY_EXE )

install( TARGETS
   token_emissions_plugin

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)
