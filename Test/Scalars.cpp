#include "TestBase.hpp"

static unsigned Hash( const char* s, size_t l )
{
  auto result = 0;
  for ( size_t ix = 0; ix < l; ix++ )
  {
    result += s[ ix ];
    result <<= 1;
  }
  return result;
}

struct ScalarTesting : ResultTesting {};

TEST_F( ScalarTesting, Single )
{
  TestResult( "14", 14 );
}

TEST_F( ScalarTesting, Add )
{
  TestResult( "14+7", 14 + 7 );
}

TEST_F( ScalarTesting, Sub )
{
  TestResult( "14-7", 14 - 7 );
}

TEST_F( ScalarTesting, Mul )
{
  TestResult( "14*7", 14 * 7 );
}

TEST_F( ScalarTesting, Div )
{
  TestResult( "14/7", real( 14 ) / 7 );
}

TEST_F( ScalarTesting, DivByZero )
{
  TestResult( "14/0", std::numeric_limits< real >::infinity() );
}

TEST_F( ScalarTesting, Pow )
{
  TestResult( "14^7", std::pow( real( 14 ), real( 7 ) ) );
}

TEST_F( ScalarTesting, Mod )
{
  TestResult( "14%7", std::fmod( real( 14 ), real( 7 ) ) );
}

TEST_F( ScalarTesting, ModByZero )
{
  TestResult( "14%0", std::numeric_limits< real >::quiet_NaN() );
}

TEST_F( ScalarTesting, Negate )
{
  TestResult( "-7", -7 );
}

TEST_F( ScalarTesting, NegateAndSubtract )
{
  TestResult( "-7-7", -14 );
}

TEST_F( ScalarTesting, LongExpression )
{
  TestResult( "-7-7*3/(-12/2)*12+32/14-5", -7-7*3/(-real(12)/2)*12+real(32)/14-5 );
}

TEST_F( ScalarTesting, Spaces1 )
{
  TestResult( "14 /7", real( 14 ) / 7 );
}

TEST_F( ScalarTesting, Spaces2 )
{
  TestResult( "14/ 7", real( 14 ) / 7 );
}

TEST_F( ScalarTesting, Spaces3 )
{
  TestResult( "14 / 7", real( 14 ) / 7 );
}

TEST_F( ScalarTesting, Spaces4 )
{
  TestResult( "   14       /           7   ", real( 14 ) / 7 );
}

TEST_F( ScalarTesting, Tabs )
{
  TestResult( "\t\t14\t\t\t/\t7\t\t\t\t\t", real( 14 ) / 7 );
}

TEST_F( ScalarTesting, Hash )
{
  SetHashFunction( Hash );
  TestResult( "'asd'", real( Hash( "asd", 3 ) ) );
  SetHashFunction( nullptr );
}

TEST_F( ScalarTesting, HashWithOp )
{
  SetHashFunction( Hash );
  TestResult( "'asd'+3", real( Hash( "asd", 3 ) ) + 3 );
  SetHashFunction( nullptr );
}

TEST_F( ScalarTesting, HashError )
{
  TestError( "'asd'", ErrorType::InvalidArguments );
}
