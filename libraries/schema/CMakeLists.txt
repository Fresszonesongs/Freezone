file(GLOB HEADERS "include/freezone/schema/*.hpp")
add_library( freezone_schema schema.cpp ${HEADERS} )
target_link_libraries( freezone_schema fc )
target_include_directories( freezone_schema PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )

install( TARGETS
   freezone_schema

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)
install( FILES ${HEADERS} DESTINATION "include/freezone/schema" )
