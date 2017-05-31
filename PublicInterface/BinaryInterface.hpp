#pragma once

#include "Expression.hpp"
#include "Errors.hpp"

extern "C"
{

DLLIFACE const SimpleMath::Expression* ParseExpression( const char* expressionText, size_t length, SimpleMath::ParseErrorDetails& error, const SimpleMath::EvaluateContext& context );
DLLIFACE void                          WasteExpression( const SimpleMath::Expression* expression );

}