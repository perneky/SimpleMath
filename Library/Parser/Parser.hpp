#pragma once

#include "CommonTypes.hpp"
#include "ExpressionTree/Node.hpp"

namespace SimpleMath
{
namespace Parser
{

class Parser
{
public:
  static ExpressionTree::Node::Unique Parse( const char* expressionText
                                           , size_t length
                                           , char* errorBuffer
                                           , size_t errorBufferSize
                                           , const EvaluateContext& context );
};

}
}