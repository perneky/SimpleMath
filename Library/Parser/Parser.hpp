#pragma once

#include "CommonTypes.hpp"
#include "ExpressionTree/Node.hpp"
#include "Errors.hpp"

namespace SimpleMath
{
namespace Parser
{

class Parser
{
public:
  static ExpressionTree::Node::Unique Parse( const char* expressionText
                                           , size_t length
                                           , ParseErrorDetails& error
                                           , const EvaluateContext& context );
};

}
}