#pragma once

#include "../ThirdParty/gtest/googletest/include/gtest/gtest.h"
#include "SimpleMath.hpp"
#include <cstring>

struct ResultTesting : testing::Test
{
  virtual void SetUp() override
  {
    context.variables.instances     = externalVariables;
    context.variables.instanceCount = sizeof( externalVariables ) / sizeof externalVariables[ 0 ];
  }

  virtual void TearDown() override
  {
  }

  void TestResultImpl( const char* expression, size_t dim, const real* r )
  {
    SimpleMath::ParseErrorDetails error;
    auto script = ParseExpression( expression, std::strlen( expression ), error, context );
    ASSERT_NE( nullptr, script );

    EvalResult result;
    auto resultDims = script->Evaluate( context, result );
    EXPECT_EQ( dim, resultDims );

    for ( size_t d = 0; d < resultDims; ++d )
    {
      if ( std::isnan( r[ d ] ) )
        EXPECT_TRUE( std::isnan( result[ d ] ) );
      else if ( std::isinf( r[ d ] ) )
        EXPECT_TRUE( std::isinf( result[ d ] ) );
      else
        EXPECT_NEAR( r[ d ], result[ d ], std::numeric_limits< real >::epsilon() * 10 );
    }

    WasteExpression( script );
  }

  void TestError( const char* expression, SimpleMath::ErrorType type )
  {
    SimpleMath::ParseErrorDetails error;

    auto script = ParseExpression( expression, std::strlen( expression ), error, context );
    ASSERT_EQ( nullptr, script );

    EXPECT_EQ( type, error.type );
  }

  template< typename T1 >
  void TestResult( const char* expression, T1 r1 )
  {
    const real r[] = { real( r1 ), 0, 0, 0 };
    TestResultImpl( expression, 1, r );
  }
  template< typename T1, typename T2 >
  void TestResult( const char* expression, T1 r1, T2 r2 )
  {
    const real r[] = { real( r1 ), real( r2 ), 0, 0 };
    TestResultImpl( expression, 2, r );
  }
  template< typename T1, typename T2, typename T3 >
  void TestResult( const char* expression, T1 r1, T2 r2, T3 r3 )
  {
    const real r[] = { real( r1 ), real( r2 ), real( r3 ), 0 };
    TestResultImpl( expression, 3, r );
  }
  template< typename T1, typename T2, typename T3, typename T4 >
  void TestResult( const char* expression, T1 r1, T2 r2, T3 r3, T4 r4 )
  {
    const real r[] = { real( r1 ), real( r2 ), real( r3 ), real( r4 ) };
    TestResultImpl( expression, 4, r );
  }

  SimpleMath::EvaluateContext context;

  const SimpleMath::ExternalVariables::Variable externalVariables[ 2 ] =
  {
    SimpleMath::ExternalVariables::Variable( "test_number1", real( 7  ) ),
    SimpleMath::ExternalVariables::Variable( "test_number2", real( 14 ) ),
  };
};
