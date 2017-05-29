#include <../ThirdParty/gtest/googletest/include/gtest/gtest.h>
#include "SimpleMath.hpp"

using namespace testing;

static const SimpleMath::ExternalVariables::Variable externalVariables[] =
{
  SimpleMath::ExternalVariables::Variable( "cool_number", real( 14 ) ),
};

struct ResultTesting : Test
{
  virtual void SetUp() override
  {
    context.variables.instances     = externalVariables;
    context.variables.instanceCount = sizeof( externalVariables ) / sizeof externalVariables[ 0 ];
  }

  virtual void TearDown() override
  {
  }

  void TestResult( const char* expression, size_t dim, const real* r )
  {
    auto script = ParseExpression( expression, std::strlen( expression ), nullptr, 0, context );
    ASSERT_NE( nullptr, script );

    EvalResult result;
    auto resultDims = script->Evaluate( context, result );
    EXPECT_EQ( dim, resultDims );

    for ( size_t d = 0; d < resultDims; ++d )
      EXPECT_EQ( r[ d ], result[ d ] );

    WasteExpression( script );
  }

  void TestResult( const char* expression, real r1 )
  {
    const real r[] = { r1, 0, 0, 0 };
    TestResult( expression, 1, r );
  }
  void TestResult( const char* expression, real r1, real r2 )
  {
    const real r[] = { r1, r2, 0, 0 };
    TestResult( expression, 2, r );
  }
  void TestResult( const char* expression, real r1, real r2, real r3 )
  {
    const real r[] = { r1, r2, r3, 0 };
    TestResult( expression, 3, r );
  }
  void TestResult( const char* expression, real r1, real r2, real r3, real r4 )
  {
    const real r[] = { r1, r2, r3, r4 };
    TestResult( expression, 4, r );
  }

  SimpleMath::EvaluateContext context;
};

TEST_F( ResultTesting, SingleScalar )
{
  TestResult( "14", 14 );
}

int main( int argc, char* argv[] )
{
  InitGoogleTest( &argc, argv );

  return RUN_ALL_TESTS();
}