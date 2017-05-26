file( GLOB SandboxHeaders "*.hpp" )
file( GLOB SandboxSources "*.cpp" )

add_executable( Sandbox ${SandboxHeaders} ${SandboxSources} )

target_include_directories( Sandbox PUBLIC . )
target_include_directories( Sandbox PUBLIC ../PublicInterface )

add_dependencies( Sandbox SimpleMath )
target_link_libraries( Sandbox SimpleMath )
