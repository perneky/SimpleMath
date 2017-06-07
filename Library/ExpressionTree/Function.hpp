#pragma once

#include "Node.hpp"

namespace SimpleMath
{
namespace ExpressionTree
{

class FunctionBase : public Node
{
public:
  FunctionBase()
  {
  }

  virtual void SetOperand( size_t index, Node* op ) noexcept = 0;
};

template< size_t argCount >
class Function : public FunctionBase
{
public:
  Function( ExternalFunctions::Function func )
    : func( func )
  {
    for ( auto& op : operands )
      op = nullptr;
  }

  virtual ~Function()
  {
    for ( auto& op : operands )
      if ( op )
        delete op;
  }

  virtual void SetOperand( size_t index, Node* op ) noexcept override
  {
    operands[ index ] = op;
  }

  virtual size_t Validate( const EvaluateContext& context ) const override
  {
    size_t dims[ argCount ];
    for ( size_t argIndex = 0; argIndex < argCount; ++argIndex )
      dims[ argIndex ] = operands[ argIndex ]->Validate( context );

    return func.validator( context, argCount, dims );
  }

  virtual size_t Evaluate( const EvaluateContext& context, EvalResult result ) const noexcept override
  {
    EvalResult args[ argCount ];
    size_t     dims[ argCount ];
    for ( size_t argIndex = 0; argIndex < argCount; ++argIndex )
      dims[ argIndex ] = operands[ argIndex ]->Evaluate( context, args[ argIndex ] );

    return func.value( context, argCount, dims, args, result );
  }

  virtual bool IsConstant() const noexcept override
  {
    bool isConst = func.isConst;
    for ( size_t argIndex = 0; argIndex < argCount; ++argIndex )
      isConst &= operands[ argIndex ]->IsConstant();

    return isConst;
  }

  virtual void OptimizeChildren( const EvaluateContext& context ) override
  {
    for ( size_t argIndex = 0; argIndex < argCount; ++argIndex )
      operands[ argIndex ] = Optimize( operands[ argIndex ], context );
  }

protected:
  ExternalFunctions::Function func;

  Node* operands[ argCount ];
};

template<>
class Function< 0 > : public Node
{
public:
  Function( ExternalFunctions::Function func )
    : func( func )
  {
  }

  virtual size_t Validate( const EvaluateContext& context ) const override
  {
    return func.validator( context, 0, nullptr );
  }

  virtual size_t Evaluate( const EvaluateContext& context, EvalResult result ) const noexcept override
  {
    return func.value( context, 0, nullptr, nullptr, result );
  }

  virtual bool IsConstant() const noexcept override
  {
    return func.isConst;
  }

protected:
  ExternalFunctions::Function func;
};

}
}