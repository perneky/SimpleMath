#pragma once

#include "CommonTypes.hpp"
#include <exception>
#include <cstdarg>

namespace SimpleMath
{
namespace Parser
{

class CompileError : public std::exception
{
public:
  CompileError( const char* format, ... )
  {
    va_list args;
    va_start( args, format );

    std::vsnprintf( message, sizeof( message ), format, args );

    va_end( args );
  }
  CompileError( const char* format, va_list args )
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
