#include "TestBase.hpp"

using namespace SimpleMath;

struct CustomAllocTesting : ResultTesting {};

static size_t                    memCount = 0;
static std::map< void*, size_t > memMap;

TEST_F( CustomAllocTesting, Basic )
{
  memCount = 0;
  memMap.clear();

  SetAllocator( []( size_t n )
  {
    memCount += n;
    auto p = std::malloc( n );
    memMap[ p ] = n;
    return p;
  },
  []( void* p )
  {
    memCount -= memMap[ p ];
    memMap.erase( p );
    std::free( p );
  } );

  const char expression[] = "3*4";

  EvaluateContext   context;
  ParseErrorDetails error;
  auto script = ParseExpression( expression, std::strlen( expression ), error, context );
  ASSERT_NE( nullptr, script );

  EXPECT_NE( 0, memCount );

  EvalResult result;
  auto resultDims = script->Evaluate( context, result );
  EXPECT_EQ( 1, resultDims );
  EXPECT_NEAR( 12, result[ 0 ], std::numeric_limits< real >::epsilon() * 10 );

  WasteExpression( script );

  EXPECT_EQ( 0, memCount );

  SetAllocator( nullptr, nullptr );
}
