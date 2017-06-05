#pragma once

#include "../ThirdParty/gtest/googletest/include/gtest/gtest.h"
#include "SimpleMath.hpp"
#include "ExpressionTree/ValidationError.hpp"
#include <cstring>

using namespace SimpleMath;

struct ResultTesting : testing::Test
{
  virtual void SetUp() override
  {
    context.variables.instances     = externalVariables;
    context.variables.instanceCount = sizeof( externalVariables ) / sizeof externalVariables[ 0 ];
    context.functions.instances     = externalFunctions;
    context.functions.instanceCount = sizeof( externalFunctions ) / sizeof externalFunctions[ 0 ];
  }

  virtual void TearDown() override
  {
  }

  void TestResultImpl( const char* expression, size_t dim, const real* r )
  {
    ParseErrorDetails error;
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

  void TestError( const char* expression, ErrorType type )
  {
    ParseErrorDetails error;

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

  EvaluateContext context;

  const ExternalVariables::Variable externalVariables[ 2 ] =
  {
    ExternalVariables::Variable( "test_number1", real( 7  ) ),
    ExternalVariables::Variable( "test_number2", real( 14 ) ),
  };

  static size_t ExternalTestFunction( const EvaluateContext& context, size_t argCount, const size_t* elementsCount, const EvalResult* args, EvalResult result )
  {
    assert( argCount == 1 );

    for ( size_t elementIx = 0; elementIx < elementsCount[ 0 ]; ++elementIx )
      result[ elementIx ] = args[ 0 ][ elementIx ] + 1;

    return elementsCount[ 0 ];
  }
  static size_t ValidateTest( const EvaluateContext& /*context*/, size_t argCount, const size_t* elementsCount )
  {
    if ( argCount != 1 )
      throw ExpressionTree::ValidationError( ErrorType::InvalidArguments, "test function takes only one argument with any number of dimensions." );

    return elementsCount[ 0 ];
  }

  const ExternalFunctions::Function externalFunctions[ 1 ] =
  {
    ExternalFunctions::Function( "test", ExternalTestFunction, ValidateTest, 1 ),
  };
};
