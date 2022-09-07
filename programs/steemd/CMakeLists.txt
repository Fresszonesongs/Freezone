add_executable( freezoned main.cpp )
if( UNIX AND NOT APPLE )
  set(rt_library rt )
endif()

find_package( Gperftools QUIET )
if( GPERFTOOLS_FOUND )
    message( STATUS "Found gperftools; compiling freezoned with TCMalloc")
    list( APPEND PLATFORM_SPECIFIC_LIBS tcmalloc )
endif()

if( freezone_STATIC_BUILD )
   target_link_libraries( freezoned PRIVATE
      "-static-libstdc++ -static-libgcc"
      appbase
      freezone_utilities
      freezone_plugins
      ${CMAKE_DL_LIBS}
      ${PLATFORM_SPECIFIC_LIBS}
   )
else( freezone_STATIC_BUILD )
   target_link_libraries( freezoned PRIVATE
      appbase
      freezone_utilities
      freezone_plugins
      ${CMAKE_DL_LIBS}
      ${PLATFORM_SPECIFIC_LIBS}
   )
endif( freezone_STATIC_BUILD )

if( CLANG_TIDY_EXE )
   set_target_properties(
      freezoned PROPERTIES
      CXX_CLANG_TIDY "${DO_CLANG_TIDY}"
   )
endif( CLANG_TIDY_EXE )

install( TARGETS
   freezoned

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)
