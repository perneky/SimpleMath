#pragma once

#include <memory>
#include <vector>
#include <string>
#include <map>

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

#define OnScopeExit( f ) auto scopeExit_##__COUNTER__ = MakeScopeExit( [&]{ f; } )