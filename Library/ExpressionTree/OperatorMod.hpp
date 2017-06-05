#pragma once

#include "OperatorBase.hpp"

#include <cmath>
#include <cassert>

namespace SimpleMath
{
namespace ExpressionTree
{

class OperatorMod : public OperatorBase
{
public:
  virtual int GetOrder() const noexcept override
  {
    return 4;
  }

  virtual size_t Validate( const EvaluateContext& context ) const override
  {
    auto ldim = leftOperand ->Validate( context );
    auto rdim = rightOperand->Validate( context );

    if ( ldim != rdim && rdim != 1 )
      throw ValidationError( ErrorType::InvalidDimensions, "For modulo operator, the dimension of operands should be equal, or one for the right operand ( %d, %d ).", int( ldim ), int( rdim ) );

    return ldim;
  }

  virtual size_t Evaluate( const EvaluateContext& context, EvalResult result ) const noexcept override
  {
    EvalResult a, b;
    auto ldim = leftOperand ->Evaluate( context, a );
    auto rdim = rightOperand->Evaluate( context, b );

    assert( ( ldim == rdim || rdim == 1 ) && "Right dimensions should be equal to the left, or one." );

    if ( ldim == rdim )
    {
      result[ 0 ] = Mod( a[ 0 ], b[ 0 ] );
      if ( ldim > 1 )
      {
        result[ 1 ] = Mod( a[ 1 ], b[ 1 ] );
        if ( ldim > 2 )
        {
          result[ 2 ] = Mod( a[ 2 ], b[ 2 ] );
          if ( ldim > 3 )
          {
            result[ 3 ] = Mod( a[ 3 ], b[ 3 ] );
          }
        }
      }
    }
    else
    {
      result[ 0 ] = Mod( a[ 0 ], b[ 0 ] );
      if ( ldim > 1 )
      {
        result[ 1 ] = Mod( a[ 1 ], b[ 0 ] );
        if ( ldim > 2 )
        {
          result[ 2 ] = Mod( a[ 2 ], b[ 0 ] );
          if ( ldim > 3 )
          {
            result[ 3 ] = Mod( a[ 3 ], b[ 0 ] );
          }
        }
      }
    }

    return ldim;
  }

private:
  static real Mod( real a, real b )
  {
    return ( b == 0 ) ? std::numeric_limits< real >::quiet_NaN() : std::fmod( a, b );
  }
};

}
}