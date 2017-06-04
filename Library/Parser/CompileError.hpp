#pragma once

#include "CommonTypes.hpp"
#include "../CommonHeaders/InternalTypes.hpp"

#include <cstdarg>

namespace SimpleMath
{
namespace Parser
{

class CompileError : public Exception
{
public:
  CompileError( ErrorType type, const char* format, ... )
    : Exception( type )
  {
    va_list args;
    va_start( args, format );

    std::vsnprintf( message, sizeof( message ), format, args );

    va_end( args );
  }
  CompileError( ErrorType type, const char* format, va_list args )
    : Exception( type )
  {
    std::vsnprintf( message, sizeof( message ), format, args );
  }

  virtual const char* what() const noexcept
  {
    return message;
  }

private:
  char message[ 1000 ];
};

}
}
