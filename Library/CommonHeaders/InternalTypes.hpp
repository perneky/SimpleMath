#pragma once

#include <memory>
#include <vector>
#include <string>
#include <map>
#include "Errors.hpp"

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

template < typename T >
auto MakeScopeExit( T&& f )
{
  return ScopeExit< T >( std::forward< T >( f ) );
};

#define OnScopeExit( f ) auto scopeExit_##__COUNTER__ = ::SimpleMath::MakeScopeExit( [&]{ f; } )

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