#pragma once

#include "Context.hpp"
#include "Node.hpp"
#include "ConstantValue.hpp"

namespace SimpleMath
{
namespace ExpressionTree
{

static Node* Optimize( Node* node, const EvaluateContext& context )
{
  if ( node->IsConstant() )
  {
    EvalResult result;
    auto dims = node->Evaluate( context, result );
    switch ( dims )
    {
    case 1:
      delete node;
      return new ConstantValue< 1 >( result );
    case 2:
      delete node;
      return new ConstantValue< 2 >( result );
    case 3:
      delete node;
      return new ConstantValue< 3 >( result );
    case 4:
      delete node;
      return new ConstantValue< 4 >( result );
    default:
      return node;
    }
  }

  node->OptimizeChildren( context );

  return node;
}

}
}