#pragma once

#include "OperatorBase.hpp"

namespace SimpleMath
{
namespace ExpressionTree
{

class OperatorSub : public OperatorBase
{
public:
  virtual int GetOrder() const noexcept override
  {
    return 2;
  }

  virtual size_t Evaluate( const EvaluateContext& context, EvalResult result ) const noexcept override
  {
    EvalResult a, b;
    auto ldim = leftOperand ->Evaluate( context, a );
    auto rdim = rightOperand->Evaluate( context, b );

    assert( ( ldim == rdim || rdim == 1 || ( ldim == 1 && a[ 0 ] == 0 ) ) && "Right dimensions should be equal to the left, or one." );
    if ( ldim != rdim && rdim != 1 && !( ldim == 1 && a[ 0 ] == 0 ) )
      return 0;

    if ( ldim == 1 && a[ 0 ] == 0 )
    {
      result[ 0 ] = 0 - b[ 0 ];
      if ( rdim > 1 )
      {
        result[ 1 ] = 0 - b[ 1 ];
        if ( rdim > 2 )
        {
          result[ 2 ] = 0 - b[ 2 ];
          if ( rdim > 3 )
          {
            result[ 3 ] = 0 - b[ 3 ];
          }
        }
      }

      return rdim;
    }
    else if ( ldim == rdim )
    {
      result[ 0 ] = a[ 0 ] - b[ 0 ];
      if ( ldim > 1 )
      {
        result[ 1 ] = a[ 1 ] - b[ 1 ];
        if ( ldim > 2 )
        {
          result[ 2 ] = a[ 2 ] - b[ 2 ];
          if ( ldim > 3 )
          {
            result[ 3 ] = a[ 3 ] - b[ 3 ];
          }
        }
      }
    }
    else
    {
      result[ 0 ] = a[ 0 ] - b[ 0 ];
      if ( ldim > 1 )
      {
        result[ 1 ] = a[ 1 ] - b[ 0 ];
        if ( ldim > 2 )
        {
          result[ 2 ] = a[ 2 ] - b[ 0 ];
          if ( ldim > 3 )
          {
            result[ 3 ] = a[ 3 ] - b[ 0 ];
          }
        }
      }
    }

    return ldim;
  }
};

}
}