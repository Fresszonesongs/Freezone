file(GLOB HEADERS "include/freezone/private_message/*.hpp")

add_library( freezone_private_message
             private_message_plugin.cpp
           )

target_link_libraries( freezone_private_message freezone_chain freezone_protocol freezone_app )
target_include_directories( freezone_private_message
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )

install( TARGETS
   freezone_private_message

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)
