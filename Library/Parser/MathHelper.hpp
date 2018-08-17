#pragma once

#include <cmath>

namespace SimpleMath
{

static const real piValue     = real( 3.14159265358979323846 );
static const real halfPiValue = real( 1.57079632679489661923 );

static real ToRadians( real x )
{
  return x * piValue / 180;
}

static real ToDegrees( real x )
{
  return x * 180 / piValue;
}

static real ctg( real x )
{
  return std::tan( halfPiValue - x );
}

#if SIMPLE_MATH_USE_DEGREES_FOR_ANGLES
# define CTM( x ) SimpleMath::ToRadians( (x) )
# define MTC( x ) SimpleMath::ToDegrees( (x) )
#else
# define CTM( x ) (real(x))
# define MTC( x ) (real(x))
#endif // SIMPLE_MATH_USE_DEGREES_FOR_ANGLES

}