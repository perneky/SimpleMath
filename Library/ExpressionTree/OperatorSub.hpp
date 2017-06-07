#pragma once

#include "OperatorBase.hpp"

#include <cassert>

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

  virtual size_t Validate( const EvaluateContext& context ) const override
  {
    if ( !leftOperand || !rightOperand )
      throw ValidationError( ErrorType::InternalError, "Both operand should be valid for operators." );

    auto ldim = leftOperand ->Validate( context );
    auto rdim = rightOperand->Validate( context );

    if ( ldim != rdim && rdim != 1 && ldim != 1 )
      throw ValidationError( ErrorType::InvalidDimensions, "For subtraction operator, the dimension of operands should be equal, one for the right operand, or one for the right operand with the value of zero for negation ( %d, %d ).", int( ldim ), int( rdim ) );

    if ( ldim == 1 )
      return rdim;

    return ldim;
  }

  virtual size_t Evaluate( const EvaluateContext& context, EvalResult result ) const noexcept override
  {
    EvalResult a, b;
    auto ldim = leftOperand ->Evaluate( context, a );
    auto rdim = rightOperand->Evaluate( context, b );

    assert( ( ldim == rdim || rdim == 1 || ( ldim == 1 && a[ 0 ] == 0 ) ) && "Right dimensions should be equal to the left, or one." );

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