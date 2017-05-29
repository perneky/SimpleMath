cmake_minimum_required( VERSION 3.6 )

project( SimpleMath )

option( USE_DOUBLE_PRECISION "Set to ON to make SimpleMath work with double precision floating point numbers." OFF )
option( USE_DEGREES_FOR_ANGLES "Set to ON to make SimpleMath calculate with degress for angles instead of radians." OFF )

if ( WIN32 )
  add_definitions( -DNOMINMAX )
endif()

if ( USE_DOUBLE_PRECISION )
  add_definitions( -DUSE_DOUBLE_PRECISION=1 )
else()
  add_definitions( -DUSE_DOUBLE_PRECISION=0 )
endif()

if ( USE_DEGREES_FOR_ANGLES )
  add_definitions( -DUSE_DEGREES_FOR_ANGLES=1 )
else()
  add_definitions( -DUSE_DEGREES_FOR_ANGLES=0 )
endif()

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )

#gtest
set( BUILD_GTEST ON CACHE BOOL "Only gtest is needed" FORCE )
set( BUILD_GMOCK OFF CACHE BOOL "gmock is not needed" FORCE )
set( gtest_force_shared_crt ON CACHE BOOL "SimpleMath is built using the dynamic runtime" FORCE )

add_subdirectory( ThirdParty/gtest )

#project
add_subdirectory( Library )
add_subdirectory( Sandbox )
add_subdirectory( Test )
