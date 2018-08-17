#include "BinaryInterface.hpp"
#include "ExpressionTree/ExpressionTree.hpp"
#include "Parser/Parser.hpp"

#include <cstring>
#include <cstdlib>

namespace SimpleMath
{
  CustomAllocator*   customAlloc   = nullptr;
  CustomDeallocator* customDealloc = nullptr;
}

void* operator new ( std::size_t size )
{
  auto p = SimpleMath::customAlloc ? SimpleMath::customAlloc( size ) : std::malloc( size );
  if ( !p )
    throw std::bad_alloc();

  return p;
}
void* operator new ( std::size_t size, const std::nothrow_t& nothrow_value ) noexcept
{
  return SimpleMath::customAlloc ? SimpleMath::customAlloc( size ) : std::malloc( size );
}
void* operator new[] ( std::size_t size )
{
  return ::operator new( size );
}
void* operator new[] ( std::size_t size, const std::nothrow_t& nothrow_value ) noexcept
{
  return ::operator new( size, nothrow_value );
}

void operator delete ( void* ptr ) noexcept
{
  SimpleMath::customDealloc ? SimpleMath::customDealloc( ptr ) : std::free( ptr );
}
void operator delete ( void* ptr, const std::nothrow_t& nothrow_constant ) noexcept
{
  ::operator delete( ptr );
}
void operator delete[] ( void* ptr ) noexcept
{
  ::operator delete( ptr );
}
void operator delete[] ( void* ptr, const std::nothrow_t& nothrow_constant ) noexcept
{
  ::operator delete( ptr, nothrow_constant );
}

extern "C"
{

void SetAllocator( SimpleMath::CustomAllocator allocator, SimpleMath::CustomDeallocator deallocator )
{
  SimpleMath::customAlloc   = allocator;
  SimpleMath::customDealloc = deallocator;
}

const SimpleMath::Expression* ParseExpression( const char* expressionText
                                             , size_t length
                                             , SimpleMath::ParseErrorDetails& error
                                             , const SimpleMath::EvaluateContext& context )
{
  if ( length == 0 && expressionText )
    length = std::strlen( expressionText );

  if ( auto node = SimpleMath::Parser::Parser::Parse( expressionText, length, error, context ) )
    return new SimpleMath::ExpressionTree::ExpressionTree( node );
  else
    return nullptr;
}

void WasteExpression( const SimpleMath::Expression* expression )
{
  auto realExpression = static_cast< const SimpleMath::ExpressionTree::ExpressionTree* >( expression );
  delete realExpression;
}

}
