#include "Expression.hpp"

extern "C"
{

DLLIFACE const SimpleMath::Expression* ParseExpression( const char* expressionText, size_t length, char* errorBuffer, size_t errorBufferSize, const SimpleMath::EvaluateContext& context );
DLLIFACE void                          WasteExpression( const SimpleMath::Expression* expression );

}