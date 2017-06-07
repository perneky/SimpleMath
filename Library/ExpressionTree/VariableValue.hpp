#pragma once

#include "Node.hpp"

#include <cassert>

namespace SimpleMath
{
namespace ExpressionTree
{

template< size_t dim >
class VariableValue : public Node
{
public:
  VariableValue( size_t index ) noexcept
    : index( index )
  {
  }
  virtual ~VariableValue() = default;

  virtual size_t Validate( const EvaluateContext& /*context*/ ) const override
  {
    return dim;
  }

  virtual size_t Evaluate( const EvaluateContext& context, EvalResult result ) const noexcept override
  {
    assert( index < context.variables.instanceCount && "Invalid variable index" );
    assert( dim == context.variables.instances[ index ].dimensions && "Invalid variable index" );

    const auto& var = context.variables.instances[ index ];

    result[ 0 ] = var.value[ 0 ];
    if ( dim > 1 )
    {
      result[ 1 ] = var.value[ 1 ];
      if ( dim > 2 )
      {
        result[ 2 ] = var.value[ 2 ];
        if ( dim > 3 )
        {
          result[ 3 ] = var.value[ 3 ];
        }
      }
    }

    return dim;
  }

  virtual bool IsConstant() const noexcept override
  {
    return false;
  }

private:
  size_t index;
};

}
}