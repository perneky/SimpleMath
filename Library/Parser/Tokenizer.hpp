#pragma once

#include "CommonTypes.hpp"
#include "Tokens.hpp"
#include "CommonHeaders/InternalTypes.hpp"
#include "CommonHeaders/STDAllocator.hpp"

#include <vector>

namespace SimpleMath
{
namespace Parser
{

class Tokenizer
{
public:
  struct Token
  {
    Token() = default;
    Token( const char* start, size_t length, TokenType type )
      : type( type )
      , start( start )
      , length( length )
    {
    }

    TokenType   type   = TokenType::Unknown;
    const char* start  = nullptr;
    size_t      length = 0;
  };

  using Tokens = std::vector< Token, STDAllocator< Token > >;

  Tokenizer( const char* expressionText, size_t length );
  ~Tokenizer()
  {
    printf( "" );
  }

  const Tokens& GetTokens() const noexcept { return tokens; }

private:
  Tokens tokens;
};

}
}