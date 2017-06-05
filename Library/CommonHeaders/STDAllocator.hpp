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
  using value_type = T;

  STDAllocator() = default;

  template < class U >
  STDAllocator( const STDAllocator< U >& )
  {
  }

  T* allocate( std::size_t n ) 
  {
    auto p = SimpleMath::customAlloc ? SimpleMath::customAlloc( n * sizeof( T ) ) : std::malloc( n * sizeof( T ) );
    if ( !p )
      throw std::bad_alloc();

    return static_cast< T* >( p );
  }
  void deallocate( T* ptr, std::size_t ) 
  {
    SimpleMath::customDealloc ? SimpleMath::customDealloc( ptr ) : std::free( ptr );
  }
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
