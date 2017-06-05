#include "TestBase.hpp"

using namespace SimpleMath;

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
