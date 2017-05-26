file( GLOB BinaryInterfaceSources "BinaryInterface/*.cpp" )

file( GLOB ExpressionTreeHeaders "ExpressionTree/*.hpp" )
file( GLOB ExpressionTreeSources "ExpressionTree/*.cpp" )

file( GLOB ParserHeaders "Parser/*.hpp" )
file( GLOB ParserSources "Parser/*.cpp" )

file( GLOB PublicInterfaceFiles "../PublicInterface/*.hpp" )

file( GLOB CommonHeaderFiles "CommonHeaders/*.hpp" )

source_group( "Binary interface" FILES ${BinaryInterfaceSources} )
source_group( "Public interface" FILES ${PublicInterfaceFiles} )
source_group( "Expression tree" FILES ${ExpressionTreeHeaders} ${ExpressionTreeSources} )
source_group( "Parser" FILES ${ParserHeaders} ${ParserSources} )
source_group( "Common headers" FILES ${CommonHeaderFiles} )

add_library( SimpleMath SHARED ${PublicInterfaceFiles}
                               ${ExpressionTreeHeaders} 
                               ${ExpressionTreeSources}
                               ${ParserHeaders} 
                               ${ParserSources}
                               ${CommonHeaderFiles}
                               ${BinaryInterfaceSources} )

target_include_directories( SimpleMath PUBLIC . )
target_include_directories( SimpleMath PUBLIC ../PublicInterface )

target_compile_definitions( SimpleMath PRIVATE SM_DLL )