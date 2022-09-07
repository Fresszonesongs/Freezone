file(GLOB HEADERS "include/freezone/delayed_node/*.hpp")

add_library( freezone_delayed_node
             delayed_node_plugin.cpp
           )

target_link_libraries( freezone_delayed_node freezone_chain freezone_protocol freezone_app )
target_include_directories( freezone_delayed_node
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )

if(MSVC)
  set_source_files_properties( delayed_node_plugin.cpp PROPERTIES COMPILE_FLAGS "/bigobj" )
endif(MSVC)

install( TARGETS
   freezone_delayed_node

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)
