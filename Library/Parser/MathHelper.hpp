#pragma once

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

#if USE_DEGREES_FOR_ANGLES
# define CTM( x ) ToRadians( (x) )
# define MTC( x ) ToDegrees( (x) )
#else
# define CTM( x ) (real(x))
# define MTC( x ) (real(x))
#endif // USE_DEGREES_FOR_ANGLES

}