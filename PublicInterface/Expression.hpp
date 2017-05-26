#pragma once

#include "CommonTypes.hpp"

namespace SimpleMath
{

struct EvaluateContext;

class Expression
{
public:
  virtual size_t Evaluate( const EvaluateContext& context, EvalResult result ) const noexcept = 0;

protected:
  virtual ~Expression() = default;
};

}