#pragma once

#include "CommonTypes.hpp"
#include "Errors.hpp"
#include "Context.hpp"

namespace SimpleMath
{
namespace ExpressionTree
{
  class Node;
}
namespace Parser
{

class Parser
{
public:
  static ExpressionTree::Node* Parse( const char* expressionText
                                    , size_t length
                                    , ParseErrorDetails& error
                                    , const EvaluateContext& context );
};

}
}