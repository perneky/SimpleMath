#pragma once

#include "Node.hpp"

#include <cmath>
#include <cassert>

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