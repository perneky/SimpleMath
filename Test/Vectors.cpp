#include "TestBase.hpp"

using namespace SimpleMath;

struct VectorTesting : ResultTesting {};

TEST_F( VectorTesting, Single2D )
{
  TestResult( "vector(14,7)", 14, 7 );
}

TEST_F( VectorTesting, Single3D )
{
  TestResult( "vector(14,7,7)", 14, 7, 7 );
}

TEST_F( VectorTesting, Single4D )
{
  TestResult( "vector(14,7,7,14)", 14, 7, 7, 14 );
}

TEST_F( VectorTesting, Single4DWithSpaces )
{
  TestResult( "  vector(  14 ,    7  , 7   ,  14      )    ", 14, 7, 7, 14 );
}

TEST_F( VectorTesting, Single4DWithTabs )
{
  TestResult( "\t\tvector(\t \t14 \t, \t\t\t\t7\t\t,\t7 \t , 14 )\t\t", 14, 7, 7, 14 );
}

TEST_F( VectorTesting, Single4DAdd )
{
  TestResult( "vector(14,7,7,14)+vector(7,7,14,14)", 14+7, 7+7, 7+14, 14+14 );
}

TEST_F( VectorTesting, Single4DSub )
{
  TestResult( "vector(14,7,7,14)-vector(7,7,14,14)", 14 - 7, 7 - 7, 7 - 14, 14 - 14 );
}

TEST_F( VectorTesting, Single4DMul )
{
  TestResult( "vector(14,7,7,14)*vector(7,7,14,14)", 14 * 7, 7 * 7, 7 * 14, 14 * 14 );
}

TEST_F( VectorTesting, Single4DDiv )
{
  TestResult( "vector(14,7,7,14)/vector(7,7,14,14)", real( 14 ) / 7, real( 7 ) / 7, real( 7 ) / 14, real( 14 ) / 14 );
}

TEST_F( VectorTesting, Single4DDivByZero )
{
  TestResult( "vector(14,7,7,14)/vector(7,0,0,14)", real( 14 ) / 7, std::numeric_limits< real >::infinity(), std::numeric_limits< real >::infinity(), real( 14 ) / 14 );
}

TEST_F( VectorTesting, Single4DPow )
{
  TestResult( "vector(14,7,7,14)^vector(7,7,14,14)", std::pow( real( 14 ), real( 7 ) ), std::pow( real( 7 ), real( 7 ) ), std::pow( real( 7 ), real( 14 ) ), std::pow( real( 14 ), real( 14 ) ) );
}

TEST_F( VectorTesting, Single4DMod )
{
  TestResult( "vector(14,7,7,14)%vector(7,7,14,14)", std::fmod( real( 14 ), real( 7 ) ), std::fmod( real( 7 ), real( 7 ) ), std::fmod( real( 7 ), real( 14 ) ), std::fmod( real( 14 ), real( 14 ) ) );
}

TEST_F( VectorTesting, Single4DModByZero )
{
  TestResult( "vector(14,7,7,14)%vector(7,0,0,14)", std::fmod( real( 14 ), real( 7 ) ), std::numeric_limits< real >::quiet_NaN(), std::numeric_limits< real >::quiet_NaN(), std::fmod( real( 14 ), real( 14 ) ) );
}

TEST_F( VectorTesting, NegateSingle2D )
{
  TestResult( "-vector(-14,7)", 14, -7 );
}
