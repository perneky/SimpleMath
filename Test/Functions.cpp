#pragma once

#include "TestBase.hpp"
#include "../Library/Parser/MathHelper.hpp"

struct InternalFunctionTesting : ResultTesting {};

TEST_F( InternalFunctionTesting, SinScalar )
{
  TestResult( "sin(14)", std::sin( CTM( 14 ) ) );
}

TEST_F( InternalFunctionTesting, SinVector )
{
  TestResult( "sin(vector(7,14))", std::sin( CTM( 7 ) ), std::sin( CTM( 14 ) ) );
}

TEST_F( InternalFunctionTesting, CosScalar )
{
  TestResult( "cos(14)", std::cos( CTM( 14 ) ) );
}

TEST_F( InternalFunctionTesting, CosVector )
{
  TestResult( "cos(vector(7,14))", std::cos( CTM( 7 ) ), std::cos( CTM( 14 ) ) );
}

TEST_F( InternalFunctionTesting, TanScalar )
{
  TestResult( "tan(14)", std::tan( CTM( 14 ) ) );
}

TEST_F( InternalFunctionTesting, TanVector )
{
  TestResult( "tan(vector(7,14))", std::tan( CTM( 7 ) ), std::tan( CTM( 14 ) ) );
}

TEST_F( InternalFunctionTesting, CtgScalar )
{
  TestResult( "ctg(14)", SimpleMath::ctg( CTM( 14 ) ) );
}

TEST_F( InternalFunctionTesting, CtgVector )
{
  TestResult( "ctg(vector(7,14))", SimpleMath::ctg( CTM( 7 ) ), SimpleMath::ctg( CTM( 14 ) ) );
}

TEST_F( InternalFunctionTesting, ASinScalar )
{
  TestResult( "asin(14)", std::asin( MTC( 14 ) ) );
}

TEST_F( InternalFunctionTesting, ASinVector )
{
  TestResult( "asin(vector(7,14))", std::asin( MTC( 7 ) ), std::asin( MTC( 14 ) ) );
}

TEST_F( InternalFunctionTesting, ACosScalar )
{
  TestResult( "acos(14)", std::acos( MTC( 14 ) ) );
}

TEST_F( InternalFunctionTesting, ACosVector )
{
  TestResult( "acos(vector(7,14))", std::acos( MTC( 7 ) ), std::acos( MTC( 14 ) ) );
}

TEST_F( InternalFunctionTesting, ATanScalar )
{
  TestResult( "atan(14)", std::atan( MTC( 14 ) ) );
}

TEST_F( InternalFunctionTesting, ATanVector )
{
  TestResult( "atan(vector(7,14))", std::atan( MTC( 7 ) ), std::atan( MTC( 14 ) ) );
}

TEST_F( InternalFunctionTesting, SqrtVector )
{
  TestResult( "sqrt(vector(7,14))", std::sqrt( 7 ), std::sqrt( 14 ) );
}

TEST_F( InternalFunctionTesting, SaturateVector )
{
  TestResult( "saturate(vector(0.1,-0.1,0.9,1.1))", 0.1, 0, 0.9, 1 );
}

TEST_F( InternalFunctionTesting, MinVector )
{
  TestResult( "min(vector(7,14),vector(3,16))", 3, 14 );
}

TEST_F( InternalFunctionTesting, MaxVector )
{
  TestResult( "max(vector(7,14),vector(3,16))", 7, 16 );
}

TEST_F( InternalFunctionTesting, LengthVector )
{
  TestResult( "length(vector(7,14,21))", std::sqrt( 7 * 7 + 14 * 14 + 21 * 21 ) );
}

TEST_F( InternalFunctionTesting, NormalizeVector )
{
  auto length = std::sqrt( 7 * 7 + 14 * 14 + 21 * 21 );
  TestResult( "normalize(vector(7,14,21))", 7 / length, 14 / length, 21 / length );
}

TEST_F( InternalFunctionTesting, DotVector )
{
  TestResult( "dot(vector(7,14,21),vector(6,12,18))", 7*6 + 14*12 + 21*18 );
}

TEST_F( InternalFunctionTesting, CrossVector )
{
  auto a = 14 * 18 - 21 * 12;
  auto b = 21 * 6  - 7  * 18;
  auto c = 7  * 12 - 14 * 6;

  TestResult( "cross(vector(7,14,21),vector(6,12,18))", a, b, c );
}

TEST_F( InternalFunctionTesting, DotVectorSpaces )
{
  TestResult( "  dot   (   vector   ( 7    , 14   , 21   ),    vector (   6, 12  ,   18 )  )  ", 7 * 6 + 14 * 12 + 21 * 18 );
}

TEST_F( InternalFunctionTesting, DotVectorTabs )
{
  TestResult( "\tdot\t\t(\tvector \t  (\t7 \t\t  , 14\t,\t21\t),    vector\t( \t 6, \t12\t  , \t  18 )  )\t \t", 7 * 6 + 14 * 12 + 21 * 18 );
}

