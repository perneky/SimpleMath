#include "Tokenizer.hpp"

#include <cctype>

namespace SimpleMath
{
namespace Parser
{

static size_t endOfStream = size_t( -1 );

static const char separatorCharacters [] = ",()+-*/%^ \t";
static const char whitespaceCharacters[] = " \t";
static const char numberCharacters    [] = { '0','1','2','3','4','5','6','7','8','9','.' };

template< size_t N >
static bool IsAny( char c, const char (&set)[N] )
{
  for ( size_t iter = 0; iter < N; ++iter )
    if ( c == set[ iter ] )
      return true;

  return false;
}

template< size_t N >
static size_t FindNextNotOfAny( const char* text, size_t length, size_t start, const char( &set )[ N ] )
{
  for ( size_t iter = start; iter < length; ++iter )
    if ( !IsAny( text[ iter ], set ) )
      return iter;

  return endOfStream;
}

template< size_t N >
static size_t FindNextOfAny( const char* text, size_t length, size_t start, const char( &set )[ N ] )
{
  for ( size_t iter = start; iter < length; ++iter )
    if ( IsAny( text[ iter ], set ) )
      return iter;

  return endOfStream;
}

static size_t FindNextSeparator( const char* text, size_t length, size_t start )
{
  return FindNextOfAny( text, length, start, separatorCharacters );
}

static size_t FindNextWhitespace( const char* text, size_t length, size_t start )
{
  return FindNextOfAny( text, length, start, whitespaceCharacters );
}

static size_t FindNextNotWhitespace( const char* text, size_t length, size_t start )
{
  return FindNextNotOfAny( text, length, start, whitespaceCharacters );
}

static size_t FindNextNotNumber( const char* text, size_t length, size_t start )
{
  return FindNextNotOfAny( text, length, start, numberCharacters );
}

static size_t FindNextId( const char* text, size_t length, size_t start )
{
  return FindNextOfAny( text, length, start, "'" );
}

Tokenizer::Tokenizer( const char* expressionText, size_t length )
{
  size_t nextTokenStart = FindNextNotWhitespace( expressionText, length, 0 );
  while( nextTokenStart < length )
  {
    size_t    nextTokenEnd;
    TokenType nextTokenType;

    char tokenStart = std::tolower( expressionText[ nextTokenStart ] );
    switch ( tokenStart )
    {
    case ',':
      nextTokenType = TokenType::Comma;
      nextTokenEnd  = nextTokenStart + 1;
      break;
    case '(':
      nextTokenType = TokenType::OpenParenthesis;
      nextTokenEnd  = nextTokenStart + 1;
      break;
    case ')':
      nextTokenType = TokenType::CloseParenthesis;
      nextTokenEnd  = nextTokenStart + 1;
      break;
    case '+':
    case '-':
    case '*':
    case '/':
    case '^':
    case '%':
      nextTokenType = TokenType::Operator;
      nextTokenEnd  = nextTokenStart + 1;
      break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      nextTokenType = TokenType::Number;
      nextTokenEnd  = FindNextNotNumber( expressionText, length, nextTokenStart );
      break;
    case '\'':
      nextTokenStart++;
      nextTokenType = TokenType::Id;
      nextTokenEnd  = FindNextId( expressionText, length, nextTokenStart );
      break;
    default:
      nextTokenType = TokenType::String;
      nextTokenEnd  = FindNextSeparator( expressionText, length, nextTokenStart );
      break;
    }

    size_t realEnd = nextTokenEnd < endOfStream ? nextTokenEnd : length;
    tokens.emplace_back( expressionText + nextTokenStart, realEnd - nextTokenStart, nextTokenType );

    if ( nextTokenType == TokenType::Id )
      ++nextTokenEnd;

    nextTokenStart = FindNextNotWhitespace( expressionText, length, nextTokenEnd );
  }
}

}
}