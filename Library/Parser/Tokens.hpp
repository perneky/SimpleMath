#pragma once

namespace SimpleMath
{
namespace Parser
{

enum class TokenType
{
  Unknown,
  Number,
  Operator,
  OpenParenthesis,
  CloseParenthesis,
  String,
  Id,
  Comma,
};

}
}