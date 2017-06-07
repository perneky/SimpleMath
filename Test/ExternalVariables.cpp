#include "TestBase.hpp"
#include "../Library/Parser/MathHelper.hpp"

using namespace SimpleMath;

struct ExternalVariablesTesting : ResultTesting {};

TEST_F( ExternalVariablesTesting, Standalone1 )
{
  TestResult( "test_number1", 7 );
}

TEST_F( ExternalVariablesTesting, Standalone2 )
{
  TestResult( "test_number2", 14 );
}

TEST_F( ExternalVariablesTesting, StandaloneWithSpaces )
{
  TestResult( "  test_number1    ", 7 );
}

TEST_F( ExternalVariablesTesting, StandaloneWithTabs )
{
  TestResult( "\t\ttest_number1 \t\t ", 7 );
}

TEST_F( ExternalVariablesTesting, Operators )
{
  TestResult( "test_number1+test_number2", 7         + 14 );
  TestResult( "test_number1-test_number2", 7         - 14 );
  TestResult( "test_number1*test_number2", 7         * 14 );
  TestResult( "test_number1/test_number2", real( 7 ) / 14 );
}

TEST_F( ExternalVariablesTesting, Functions )
{
  TestResult( "sin(test_number1)",              std::sin ( CTM( 7 ) ) );
  TestResult( "sqrt(test_number1)",             std::sqrt( 7 ) );
  TestResult( "min(test_number1,test_number2)", 7 );
}

TEST_F( ExternalVariablesTesting, Animating )
{
  const char expression[] = "sin(pi*time)";

  ExternalVariables::Variable externalVariables[ 1 ] =
  {
    ExternalVariables::Variable( false, "time", real( 0 ) ),
  };

  EvaluateContext context;
  context.variables.instances     = externalVariables;
  context.variables.instanceCount = sizeof( externalVariables ) / sizeof externalVariables[ 0 ];

  ParseErrorDetails error;
  auto script = ParseExpression( expression, std::strlen( expression ), error, context );
  ASSERT_NE( nullptr, script );

  EvalResult result;
  for ( real iter = 0; iter < 1; iter += real( 0.05 ) )
  {
    externalVariables[ 0 ].value[ 0 ] = iter;

    auto resultDims = script->Evaluate( context, result );
    EXPECT_EQ( 1, resultDims );
    EXPECT_NEAR( std::sin( CTM( piValue * iter ) ), result[ 0 ], std::numeric_limits< real >::epsilon() * 10 );
  }

  WasteExpression( script );
}
