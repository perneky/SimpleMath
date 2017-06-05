#pragma once

#include "../../PublicInterface/BinaryInterface.hpp"

#include <cstdlib>

namespace SimpleMath
{

extern CustomAllocator*   customAlloc;
extern CustomDeallocator* customDealloc;

template< typename T >
struct STDAllocator
{
  using value_type      = T;
  using pointer         = value_type*;
  using const_pointer   = const value_type*;
  using reference       = value_type&;
  using const_reference = const value_type&;
  using size_type       = std::size_t;
  using difference_type = std::ptrdiff_t;

  STDAllocator() = default;

  template < class U >
  STDAllocator( const STDAllocator< U >& )
  {
  }

  pointer allocate( std::size_t n ) 
  {
    auto p = SimpleMath::customAlloc ? SimpleMath::customAlloc( n * sizeof( value_type ) ) : std::malloc( n * sizeof( value_type ) );
    if ( !p )
      throw std::bad_alloc();

    return static_cast< pointer >( p );
  }
  void deallocate( pointer p, std::size_t ) 
  {
    SimpleMath::customDealloc ? SimpleMath::customDealloc( p ) : std::free( p );
  }

  template< typename... TArgs >
  void construct( pointer p, TArgs&&... args )
  {
    new( p ) T( std::forward< TArgs >( args )... );
  }
  void destroy( pointer p )
  {
    p->~T();
  }

  template< typename U >
  struct rebind 
  {
    using other = STDAllocator< U >; 
  };
};

}

template< typename T, typename U >
inline bool operator == ( const SimpleMath::STDAllocator< T >&, const SimpleMath::STDAllocator< U >& )
{
  return true;
}

template< typename T, typename U >
inline bool operator != ( const SimpleMath::STDAllocator< T >& a, const SimpleMath::STDAllocator< U >& b )
{
  return !( a == b );
}
