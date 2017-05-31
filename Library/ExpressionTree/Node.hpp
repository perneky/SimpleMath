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
  using Unique = std::unique_ptr< Node >;

  virtual ~Node() = default;

  virtual size_t Validate( const EvaluateContext& context ) const = 0;
  virtual size_t Evaluate( const EvaluateContext& context, EvalResult result ) const noexcept = 0;
};

}
}