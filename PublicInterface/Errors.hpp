#pragma once

namespace SimpleMath
{

enum class ErrorType
{
  NoError,
  UnknownError,
  InvalidDimensions,
  InvalidArguments,
  SyntaxError,
  ParenthezisMismatch,
};

struct ParseErrorDetails
{
  ErrorType type;
  char      details[ 1024 ];
};

}