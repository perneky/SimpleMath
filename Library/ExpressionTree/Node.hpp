#pragma once

#include "CommonTypes.hpp"
#include "Context.hpp"
#include "CommonHeaders/InternalTypes.hpp"

namespace SimpleMath
{
namespace ExpressionTree
{

class Node
{
public:
  virtual ~Node() = default;

  virtual size_t Validate( const EvaluateContext& context ) const = 0;
  virtual size_t Evaluate( const EvaluateContext& context, EvalResult result ) const noexcept = 0;

  virtual bool IsConstant() const noexcept = 0;

  virtual void OptimizeChildren( const EvaluateContext& context ) {}
};

}
}