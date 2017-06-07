#pragma once

namespace SimpleMath
{

enum class ErrorType
{
  NoError,
  InternalError,
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