#pragma once

#include "Expression.hpp"
#include "Errors.hpp"

namespace SimpleMath
{
  void* CustomAllocatorProto  ( size_t ) noexcept;
  void  CustomDeallocatorProto( void*  ) noexcept;
  
  using CustomAllocator   = decltype( CustomAllocatorProto   );
  using CustomDeallocator = decltype( CustomDeallocatorProto );

  using HashFunction = unsigned( *)( const char*, size_t );
}

extern "C"
{

SM_DLLIFACE void SetAllocator( SimpleMath::CustomAllocator allocator, SimpleMath::CustomDeallocator deallocator );
SM_DLLIFACE void SetHashFunction( SimpleMath::HashFunction hashFunction );

SM_DLLIFACE const SimpleMath::Expression* ParseExpression( const char* expressionText, SimpleMath::size_t length, SimpleMath::ParseErrorDetails& error, const SimpleMath::EvaluateContext& context );
SM_DLLIFACE void                          WasteExpression( const SimpleMath::Expression* expression );

}