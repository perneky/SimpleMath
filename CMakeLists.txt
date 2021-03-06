cmake_minimum_required( VERSION 3.2 )

project( SimpleMath )

#gtest
set( BUILD_GTEST ON CACHE BOOL "Only gtest is needed" FORCE )
set( BUILD_GMOCK OFF CACHE BOOL "gmock is not needed" FORCE )
set( INSTALL_GTEST OFF CACHE BOOL "Installing gtest is not needed" FORCE )
set( gtest_force_shared_crt ON CACHE BOOL "SimpleMath is built using the dynamic runtime" FORCE )

add_subdirectory( Submodules/gtest )

#Simple math
option( SIMPLE_MATH_USE_DOUBLE_PRECISION "Set to ON to make SimpleMath work with double precision floating point numbers." OFF )
option( SIMPLE_MATH_USE_DEGREES_FOR_ANGLES "Set to ON to make SimpleMath calculate with degress for angles instead of radians." OFF )

if ( WIN32 )
  add_definitions( -DNOMINMAX )
  add_definitions( -DWIN32_LEAN_AND_MEAN )
endif()

if ( SIMPLE_MATH_USE_DOUBLE_PRECISION )
  add_definitions( -DSIMPLE_MATH_USE_DOUBLE_PRECISION=1 )
else()
  add_definitions( -DSIMPLE_MATH_USE_DOUBLE_PRECISION=0 )
endif()

if ( SIMPLE_MATH_USE_DEGREES_FOR_ANGLES )
  add_definitions( -DSIMPLE_MATH_USE_DEGREES_FOR_ANGLES=1 )
else()
  add_definitions( -DSIMPLE_MATH_USE_DEGREES_FOR_ANGLES=0 )
endif()

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

add_subdirectory( Library )
add_subdirectory( Sandbox )
add_subdirectory( Test )
