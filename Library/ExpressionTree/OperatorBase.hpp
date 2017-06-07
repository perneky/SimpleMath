#pragma once

#include "Node.hpp"
#include "Optimizer.hpp"

namespace SimpleMath
{
namespace ExpressionTree
{

class OperatorBase : public Node
{
public:
  OperatorBase()
  {
  }

  virtual ~OperatorBase()
  {
    if ( leftOperand )
      delete leftOperand;
    if ( rightOperand )
      delete rightOperand;
  }

  virtual bool IsConstant() const noexcept override
  {
    return leftOperand->IsConstant() && rightOperand->IsConstant();
  }

  virtual void OptimizeChildren( const EvaluateContext& context ) override
  {
    leftOperand  = Optimize( leftOperand,  context );
    rightOperand = Optimize( rightOperand, context );
  }

  void SetLeftOperand( Node* op ) noexcept
  {
    leftOperand = op;
  }
  void SetRightOperand( Node* op ) noexcept
  {
    rightOperand = op;
  }

  virtual int GetOrder() const noexcept = 0;

protected:
  Node* leftOperand  = nullptr;
  Node* rightOperand = nullptr;
};

}
}