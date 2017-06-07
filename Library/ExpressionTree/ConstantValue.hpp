#pragma once

#include "Node.hpp"

namespace SimpleMath
{
namespace ExpressionTree
{

template< size_t dim >
class ConstantValue : public Node
{
public:
  ConstantValue( const real* inValue ) noexcept
  {
    value[ 0 ] = inValue[ 0 ];
    if ( dim > 1 )
    {
      value[ 1 ] = inValue[ 1 ];
      if ( dim > 2 )
      {
        value[ 2 ] = inValue[ 2 ];
        if ( dim > 3 )
        {
          value[ 3 ] = inValue[ 3 ];
        }
      }
    }
  }
  virtual ~ConstantValue() = default;

  virtual size_t Validate( const EvaluateContext& /*context*/ ) const override
  {
    return dim;
  }

  virtual size_t Evaluate( const EvaluateContext& /*context*/, EvalResult result ) const noexcept override
  {
    result[ 0 ] = value[ 0 ];
    if ( dim > 1 )
    {
      result[ 1 ] = value[ 1 ];
      if ( dim > 2 )
      {
        result[ 2 ] = value[ 2 ];
        if ( dim > 3 )
        {
          result[ 3 ] = value[ 3 ];
        }
      }
    }

    return dim;
  }

  virtual bool IsConstant() const noexcept override
  {
    return true;
  }

private:
  real value[ dim ];
};

}
}