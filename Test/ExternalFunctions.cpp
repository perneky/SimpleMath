#include "TestBase.hpp"
#include "../Library/Parser/MathHelper.hpp"

struct ExternalFunctionTesting : ResultTesting {};

TEST_F( ExternalFunctionTesting, Match1 )
{
  TestResult( "test(1)", 2 );
}

TEST_F( ExternalFunctionTesting, Match2 )
{
  TestResult( "test(vector(1,3,4))", 2, 4, 5 );
}

TEST_F( ExternalFunctionTesting, NotMatch1 )
{
  TestError( "test(1,3,4)", SimpleMath::ErrorType::InvalidArguments );
}

TEST_F( ExternalFunctionTesting, NotMatch2 )
{
  TestError( "test()", SimpleMath::ErrorType::InvalidArguments );
}

TEST_F( ExternalFunctionTesting, NotMatch3 )
{
  TestError( "test(vector(1,2),vector(3,4))", SimpleMath::ErrorType::InvalidArguments );
}

