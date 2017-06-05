#pragma once

#include "Expression.hpp"

#include <memory>

namespace SimpleMath
{
namespace ExpressionTree
{

class Node;

class ExpressionTree : public Expression
{
public:
  ExpressionTree( Node* root );
  virtual ~ExpressionTree() = default;

  virtual size_t Evaluate( const EvaluateContext& context, EvalResult result ) const noexcept override;

private:
  std::unique_ptr< Node > root;
};

}
}