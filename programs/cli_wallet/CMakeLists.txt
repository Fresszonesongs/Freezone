add_executable( cli_wallet main.cpp )
if( UNIX AND NOT APPLE )
  set(rt_library rt )
endif()

find_package( Gperftools QUIET )
if( GPERFTOOLS_FOUND )
    message( STATUS "Found gperftools; compiling cli_wallet with TCMalloc")
    list( APPEND PLATFORM_SPECIFIC_LIBS tcmalloc )
endif()

if( freezone_STATIC_BUILD )
   target_link_libraries( cli_wallet PRIVATE
                          "-static-libstdc++ -static-libgcc -lreadline"
                          graphene_net freezone_chain freezone_protocol freezone_utilities freezone_wallet condenser_api_plugin fc ${CMAKE_DL_LIBS} ${PLATFORM_SPECIFIC_LIBS} )
else( freezone_STATIC_BUILD )
   target_link_libraries( cli_wallet PRIVATE
                          graphene_net freezone_chain freezone_protocol freezone_utilities freezone_wallet condenser_api_plugin fc ${CMAKE_DL_LIBS} ${PLATFORM_SPECIFIC_LIBS} )
endif( freezone_STATIC_BUILD )

if( CLANG_TIDY_EXE )
   set_target_properties(
      cli_wallet PROPERTIES
      CXX_CLANG_TIDY "${DO_CLANG_TIDY}"
   )
endif( CLANG_TIDY_EXE )

if(MSVC)
  set_source_files_properties( main.cpp PROPERTIES COMPILE_FLAGS "/bigobj" )
endif(MSVC)

install( TARGETS
   cli_wallet

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)
