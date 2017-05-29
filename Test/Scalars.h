#pragma once

struct ScalarTesting : ResultTesting {};

TEST_F( ScalarTesting, Single )
{
  TestResult( "14", _14 );
}

TEST_F( ScalarTesting, Add )
{
  TestResult( "14+7", _14 + _7 );
}

TEST_F( ScalarTesting, Sub )
{
  TestResult( "14-7", _14 - _7 );
}

TEST_F( ScalarTesting, Mul )
{
  TestResult( "14*7", _14 * _7 );
}

TEST_F( ScalarTesting, Div )
{
  TestResult( "14/7", _14 / _7 );
}

TEST_F( ScalarTesting, DivByZero )
{
  TestResult( "14/0", std::numeric_limits< real >::infinity() );
}

TEST_F( ScalarTesting, Pow )
{
  TestResult( "14^7", std::pow( _14, _7 ) );
}

TEST_F( ScalarTesting, Mod )
{
  TestResult( "14%7", std::fmod( _14, _7 ) );
}

TEST_F( ScalarTesting, ModByZero )
{
  TestResult( "14%0", std::numeric_limits< real >::quiet_NaN() );
}

TEST_F( ScalarTesting, Spaces1 )
{
  TestResult( "14 /7", _14 / _7 );
}

TEST_F( ScalarTesting, Spaces2 )
{
  TestResult( "14/ 7", _14 / _7 );
}

TEST_F( ScalarTesting, Spaces3 )
{
  TestResult( "14 / 7", _14 / _7 );
}

TEST_F( ScalarTesting, Spaces4 )
{
  TestResult( "   14       /           7   ", _14 / _7 );
}

TEST_F( ScalarTesting, Tabs )
{
  TestResult( "\t\t14\t\t\t/\t7\t\t\t\t\t", _14 / _7 );
}
