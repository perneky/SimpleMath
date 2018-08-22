#include "TestBase.hpp"

struct CustomAllocTesting : ResultTesting {};

static int memCount = 0;

TEST_F( CustomAllocTesting, Basic )
{
  memCount = 0;

  SetAllocator( []( size_t n ) noexcept
  {
    memCount++;
    return std::malloc( n );
  },
  []( void* p ) noexcept
  {
    memCount--;
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
