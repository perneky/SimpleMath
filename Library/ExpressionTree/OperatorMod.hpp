#pragma once

#include "OperatorBase.hpp"

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

  virtual size_t Evaluate( const EvaluateContext& context, EvalResult result ) const noexcept override
  {
    EvalResult a, b;
    auto ldim = leftOperand ->Evaluate( context, a );
    auto rdim = rightOperand->Evaluate( context, b );

    assert( ( ldim == rdim || rdim == 1 ) && "Right dimensions should be equal to the left, or one." );
    if ( ldim != rdim && rdim != 1 )
      return 0;

    if ( ldim == rdim )
    {
      result[ 0 ] = std::fmod( a[ 0 ], b[ 0 ] );
      if ( ldim > 1 )
      {
        result[ 1 ] = std::fmod( a[ 1 ], b[ 1 ] );
        if ( ldim > 2 )
        {
          result[ 2 ] = std::fmod( a[ 2 ], b[ 2 ] );
          if ( ldim > 3 )
          {
            result[ 3 ] = std::fmod( a[ 3 ], b[ 3 ] );
          }
        }
      }
    }
    else
    {
      result[ 0 ] = std::fmod( a[ 0 ], b[ 0 ] );
      if ( ldim > 1 )
      {
        result[ 1 ] = std::fmod( a[ 1 ], b[ 0 ] );
        if ( ldim > 2 )
        {
          result[ 2 ] = std::fmod( a[ 2 ], b[ 0 ] );
          if ( ldim > 3 )
          {
            result[ 3 ] = std::fmod( a[ 3 ], b[ 0 ] );
          }
        }
      }
    }

    return ldim;
  }
};

}
}