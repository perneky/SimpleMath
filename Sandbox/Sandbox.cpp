#include "SimpleMath.hpp"
#include <iostream>
#include <cstring>
#include <string>

static const SimpleMath::ExternalVariables::Variable externalVariables[] =
{
  SimpleMath::ExternalVariables::Variable( "cool_number", SimpleMath::real( 14 ) ),
};

int main()
{
  SimpleMath::EvaluateContext context;
  context.variables.instances     = externalVariables;
  context.variables.instanceCount = sizeof( externalVariables ) / sizeof externalVariables[ 0 ];

  while ( true )
  {
    char errorBuffer[ 1024 ];

    std::cout << "Enter an expression: ";

    std::string inputExpression;
    std::getline( std::cin, inputExpression );

    if ( inputExpression.empty() )
      break;

    SimpleMath::ParseErrorDetails error;
    auto expression = ParseExpression( inputExpression.data(), inputExpression.size(), error, context );
    if ( expression )
    {
      SimpleMath::EvalResult result;
      auto dims = expression->Evaluate( context, result );
      std::cout << "\nThe result has " << std::to_string( dims ) << " dimensions: ";
      for ( size_t dim = 0; dim < dims; ++dim )
        std::cout << std::to_string( result[ dim ] ) << " ";
      std::cout << "\n\n";
      WasteExpression( expression );
    }
    else
    {
      std::cout << "Error compiling '" << inputExpression << "': " << errorBuffer << "\n\n";
    }
  }

  return 0;
}
