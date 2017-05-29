file( GLOB TestHeaders "*.hpp" )
file( GLOB TestSources "*.cpp" )

add_executable( Test ${TestHeaders} ${TestSources} )

target_include_directories( Test PUBLIC . )
target_include_directories( Test PUBLIC ../PublicInterface )

add_dependencies( Test SimpleMath gtest )
target_link_libraries( Test SimpleMath gtest )
