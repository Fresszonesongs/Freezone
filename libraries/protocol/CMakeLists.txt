file(GLOB HEADERS "include/freezone/protocol/*.hpp")
file(GLOB HF_PARTS "hardfork.d/*.hpp")

if( MSVC )
   set( hardfork_hpp_file "${CMAKE_CURRENT_SOURCE_DIR}/include/freezone/protocol/hardfork.hpp" )
   add_custom_target( build_hardfork_hpp
      COMMAND cat-parts "${CMAKE_CURRENT_SOURCE_DIR}/hardfork.d" ${hardfork_hpp_file} )
   add_dependencies( build_hardfork_hpp cat-parts )
else( MSVC )
   set( hardfork_hpp_file "${CMAKE_CURRENT_BINARY_DIR}/include/freezone/protocol/hardfork.hpp" )
   add_custom_command(
      COMMAND ${CMAKE_COMMAND} -E env PYTHONPATH=${CMAKE_CURRENT_SOURCE_DIR}/../../programs/build_helpers
      python3 -m freezone_build_helpers.cat_parts
      "${CMAKE_CURRENT_SOURCE_DIR}/hardfork.d"
      ${hardfork_hpp_file}
      OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/include/freezone/protocol/hardfork.hpp"
      DEPENDS ${HF_PARTS}
   )
endif( MSVC )

## SORT .cpp by most likely to change / break compile
add_library( freezone_protocol

             SST_operations.cpp
             SST_util.cpp
             freezone_operations.cpp
             sps_operations.cpp
             freezone_required_actions.cpp
             freezone_optional_actions.cpp
             types.cpp
             schema_types.cpp
             authority.cpp
             operations.cpp
             sign_state.cpp
             transaction.cpp
             block.cpp
             asset.cpp
             version.cpp
             get_config.cpp

             ${HEADERS}
             ${hardfork_hpp_file}
             "${CMAKE_CURRENT_BINARY_DIR}/include/freezone/protocol/hardfork.hpp"
           )

target_link_libraries( freezone_protocol freezone_schema fc )
target_include_directories( freezone_protocol
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}/include" )

if( CLANG_TIDY_EXE )
   set_target_properties(
      freezone_protocol PROPERTIES
      CXX_CLANG_TIDY "${DO_CLANG_TIDY}"
   )
endif( CLANG_TIDY_EXE )

INSTALL( TARGETS
   freezone_protocol

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)

INSTALL( FILES ${HEADERS} DESTINATION "include/freezone/protocol" )
