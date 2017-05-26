#include "BinaryInterface.hpp"
#include "ExpressionTree/ExpressionTree.hpp"
#include "Parser/Parser.hpp"
#include <cstring>

extern "C"
{

const SimpleMath::Expression* ParseExpression( const char* expressionText
                                             , size_t length
                                             , char* errorBuffer
                                             , size_t errorBufferSize
                                             , const SimpleMath::EvaluateContext& context )
{
  if ( length == 0 )
    length = std::strlen( expressionText );

  if ( auto node = SimpleMath::Parser::Parser::Parse( expressionText, length, errorBuffer, errorBufferSize, context ) )
    return new SimpleMath::ExpressionTree::ExpressionTree( move( node ) );
  else
    return nullptr;
}

void WasteExpression( const SimpleMath::Expression* expression )
{
  auto realExpression = static_cast< const SimpleMath::ExpressionTree::ExpressionTree* >( expression );
  delete realExpression;
}

}
