#pragma once

namespace SimpleMath
{

struct EvaluateContext;

struct ExternalVariables
{
  struct Variable
  {
    Variable( const char* name, real inValue0 )
      : name ( name  )
      , dimensions( 1 )
    {
      value[ 0 ] = inValue0;
    }
    Variable( const char* name, real inValue0, real inValue1 )
      : name( name )
      , dimensions( 2 )
    {
      value[ 0 ] = inValue0;
      value[ 1 ] = inValue1;
    }
    Variable( const char* name, real inValue0, real inValue1, real inValue2 )
      : name( name )
      , dimensions( 3 )
    {
      value[ 0 ] = inValue0;
      value[ 1 ] = inValue1;
      value[ 2 ] = inValue2;
    }
    Variable( const char* name, real inValue0, real inValue1, real inValue2, real inValue3 )
      : name( name )
      , dimensions( 4 )
    {
      value[ 0 ] = inValue0;
      value[ 1 ] = inValue1;
      value[ 2 ] = inValue2;
      value[ 3 ] = inValue3;
    }

    const char* name;
    size_t      dimensions;
    real        value[ 4 ];
  };

  ExternalVariables() = default;

  template< size_t variableCount >
  ExternalVariables( const Variable (&instances)[ variableCount ] )
    : instances( instances )
    , instanceCount( variableCount )
  {
  }

  const Variable* instances     = nullptr;
  size_t          instanceCount = 0;
};

struct ExternalFunctions
{
  using FunctionSignature = size_t( * )( const EvaluateContext&, size_t, const size_t*, const EvalResult*, EvalResult );

  struct Function
  {
    Function( const char* name, FunctionSignature func, size_t argCount )
      : name    ( name     )
      , value   ( func     )
      , argCount( argCount )
    {
    }

    const char*       name;
    FunctionSignature value;
    size_t            argCount;
  };

  const Function* instances     = nullptr;
  size_t          instanceCount = 0;
};

struct EvaluateContext
{
  ExternalVariables variables;
  ExternalFunctions functions;
};

}