#pragma once

#include "OperatorBase.hpp"

#include <cassert>

namespace SimpleMath
{
namespace ExpressionTree
{

class OperatorNegate : public OperatorBase
{
public:
  virtual int GetOrder() const noexcept override
  {
    return 3;
  }

  virtual size_t Validate( const EvaluateContext& context ) const override
  {
    if ( !rightOperand )
      throw ValidationError( ErrorType::InternalError, "Right operand should be valid for negation." );

    return rightOperand->Validate( context );
  }

  virtual size_t Evaluate( const EvaluateContext& context, EvalResult result ) const noexcept override
  {
    EvalResult b;
    auto rdim = rightOperand->Evaluate( context, b );

    result[ 0 ] = -b[ 0 ];
    if ( rdim > 1 )
    {
      result[ 1 ] = -b[ 1 ];
      if ( rdim > 2 )
      {
        result[ 2 ] = -b[ 2 ];
        if ( rdim > 3 )
        {
          result[ 3 ] = -b[ 3 ];
        }
      }
    }

    return rdim;
  }

  virtual bool IsConstant() const noexcept override
  {
    return rightOperand->IsConstant();
  }

  virtual void OptimizeChildren( const EvaluateContext& context ) override
  {
    rightOperand = Optimize( rightOperand, context );
  }
};

}
}