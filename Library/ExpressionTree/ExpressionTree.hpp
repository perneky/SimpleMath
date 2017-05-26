#pragma once

#include "Expression.hpp"
#include "Node.hpp"

namespace SimpleMath
{
namespace ExpressionTree
{

class Node;

class ExpressionTree : public Expression
{
public:
  ExpressionTree( Node::Unique root );
  virtual ~ExpressionTree() = default;

  virtual size_t Evaluate( const EvaluateContext& context, EvalResult result ) const noexcept override;

private:
  Node::Unique root;
};

}
}