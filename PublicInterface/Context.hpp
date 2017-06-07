#pragma once

#include <cstring>

namespace SimpleMath
{

struct EvaluateContext;

struct ExternalVariables
{
  struct Variable
  {
    Variable( bool isConst, const char* name, real inValue0 )
      : name ( name  )
      , nameLength( std::strlen( name ) )
      , dimensions( 1 )
      , isConst( isConst )
    {
      value[ 0 ] = inValue0;
    }
    Variable( bool isConst, const char* name, real inValue0, real inValue1 )
      : name( name )
      , nameLength( std::strlen( name ) )
      , dimensions( 2 )
      , isConst( isConst )
    {
      value[ 0 ] = inValue0;
      value[ 1 ] = inValue1;
    }
    Variable( bool isConst, const char* name, real inValue0, real inValue1, real inValue2 )
      : name( name )
      , nameLength( std::strlen( name ) )
      , dimensions( 3 )
      , isConst( isConst )
    {
      value[ 0 ] = inValue0;
      value[ 1 ] = inValue1;
      value[ 2 ] = inValue2;
    }
    Variable( bool isConst, const char* name, real inValue0, real inValue1, real inValue2, real inValue3 )
      : name( name )
      , nameLength( std::strlen( name ) )
      , dimensions( 4 )
      , isConst( isConst )
    {
      value[ 0 ] = inValue0;
      value[ 1 ] = inValue1;
      value[ 2 ] = inValue2;
      value[ 3 ] = inValue3;
    }

    const char* name;
    size_t      nameLength;
    size_t      dimensions;
    real        value[ 4 ];
    bool        isConst;
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
  using FunctionSignature   = size_t( * )( const EvaluateContext&, size_t, const size_t*, const EvalResult*, EvalResult );
  using ValidationSignature = size_t( * )( const EvaluateContext&, size_t, const size_t* );

  struct Function
  {
    Function( bool isConst, const char* name, FunctionSignature func, ValidationSignature valFunc, size_t argCount )
      : name( name )
      , nameLength( std::strlen( name ) )
      , value( func )
      , validator( valFunc )
      , argCount( argCount )
      , isConst( isConst )
    {
    }

    const char*         name;
    size_t              nameLength;
    FunctionSignature   value;
    ValidationSignature validator;
    size_t              argCount;
    bool                isConst;
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