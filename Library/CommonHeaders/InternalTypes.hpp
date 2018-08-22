#pragma once

#include "Errors.hpp"

#include <memory>
#include <exception>

namespace SimpleMath
{

template < typename T >
struct ScopeExit
{
  ScopeExit( T&& f )
    : f( std::move( f ) )
  {
  }
  ~ScopeExit()
  {
    f();
  }

  T f;
};

class Exception : public std::exception
{
public:
  Exception( ErrorType type )
    : type( type )
  {
  }

  ErrorType GetErrorType() const noexcept
  {
    return type;
  }

private:
  ErrorType type;
};

}