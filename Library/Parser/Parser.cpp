// The current implementation of the allocator matching seems to be broken.
// Got to check this later.
#ifdef _MSC_VER
# define _ENFORCE_MATCHING_ALLOCATORS 0
#endif // _MSC_VER

#include "Parser.hpp"

#include "ExpressionTree/ConstantValue.hpp"
#include "ExpressionTree/VariableValue.hpp"
#include "ExpressionTree/OperatorAdd.hpp"
#include "ExpressionTree/OperatorSub.hpp"
#include "ExpressionTree/OperatorMul.hpp"
#include "ExpressionTree/OperatorDiv.hpp"
#include "ExpressionTree/OperatorPow.hpp"
#include "ExpressionTree/OperatorMod.hpp"
#include "ExpressionTree/OperatorNegate.hpp"
#include "ExpressionTree/Function.hpp"
#include "Tokenizer.hpp"
#include "CompileError.hpp"
#include "MathHelper.hpp"
#include "CommonHeaders/STDAllocator.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstring>
#include <cctype>
#include <map>
#include <memory>

namespace SimpleMath
{
namespace Parser
{

static const ExternalVariables::Variable internalVariableList[] =
{
  ExternalVariables::Variable( true, "pi",        piValue     ),
  ExternalVariables::Variable( true, "two_pi",    piValue * 2 ),
  ExternalVariables::Variable( true, "half_pi",   halfPiValue ),

  ExternalVariables::Variable( true, "vec2_zero",  0, 0 ),
  ExternalVariables::Variable( true, "vec2_one",   1, 1 ),
  ExternalVariables::Variable( true, "vec2_xaxis", 1, 0 ),
  ExternalVariables::Variable( true, "vec2_yaxis", 0, 1 ),

  ExternalVariables::Variable( true, "vec3_zero",  0, 0, 0 ),
  ExternalVariables::Variable( true, "vec3_one",   1, 1, 1 ),
  ExternalVariables::Variable( true, "vec3_xaxis", 1, 0, 0 ),
  ExternalVariables::Variable( true, "vec3_yaxis", 0, 1, 0 ),
  ExternalVariables::Variable( true, "vec3_zaxis", 0, 0, 1 ),

  ExternalVariables::Variable( true, "vec4_zero",  0, 0, 0, 0 ),
  ExternalVariables::Variable( true, "vec4_one",   1, 1, 1, 1 ),
  ExternalVariables::Variable( true, "vec4_xaxis", 1, 0, 0, 0 ),
  ExternalVariables::Variable( true, "vec4_yaxis", 0, 1, 0, 0 ),
  ExternalVariables::Variable( true, "vec4_zaxis", 0, 0, 1, 0 ),
  ExternalVariables::Variable( true, "vec4_waxis", 0, 0, 0, 1 ),
};

static const ExternalVariables internalConstants( internalVariableList );

static size_t WrapMin( const EvaluateContext& context, size_t argCount, const size_t* elementsCount, const EvalResult* args, EvalResult result )
{
  assert( argCount == 2 );
  assert( elementsCount[ 0 ] == elementsCount[ 1 ] );

  for ( size_t elementIx = 0; elementIx < elementsCount[ 0 ]; ++elementIx )
    result[ elementIx ] = std::min( args[ 0 ][ elementIx ], args[ 1 ][ elementIx ] );

  return elementsCount[ 0 ];
}
static size_t ValidateMin( const EvaluateContext& /*context*/, size_t argCount, const size_t* elementsCount )
{
  if ( argCount != 2 )
    throw ExpressionTree::ValidationError( ErrorType::InvalidArguments, "min function takes two arguments with the same number of dimensions." );
  if ( elementsCount[ 0 ] != elementsCount[ 1 ] )
    throw ExpressionTree::ValidationError( ErrorType::InvalidDimensions, "min function takes two arguments with the same number of dimensions." );

  return elementsCount[ 0 ];
}

static size_t WrapMax( const EvaluateContext& context, size_t argCount, const size_t* elementsCount, const EvalResult* args, EvalResult result )
{
  assert( argCount == 2 );
  assert( elementsCount[ 0 ] == elementsCount[ 1 ] );

  for ( size_t elementIx = 0; elementIx < elementsCount[ 0 ]; ++elementIx )
    result[ elementIx ] = std::max( args[ 0 ][ elementIx ], args[ 1 ][ elementIx ] );

  return elementsCount[ 0 ];
}
static size_t ValidateMax( const EvaluateContext& /*context*/, size_t argCount, const size_t* elementsCount )
{
  if ( argCount != 2 ) 
    throw ExpressionTree::ValidationError( ErrorType::InvalidArguments, "max function takes two arguments with the same number of dimensions." );
  if ( elementsCount[ 0 ] != elementsCount[ 1 ] )
    throw ExpressionTree::ValidationError( ErrorType::InvalidDimensions, "max function takes two arguments with the same number of dimensions." );

  return elementsCount[ 0 ];
}

static size_t WrapSaturate( const EvaluateContext& context, size_t argCount, const size_t* elementsCount, const EvalResult* args, EvalResult result )
{
  assert( argCount == 1 );

  for ( size_t elementIx = 0; elementIx < elementsCount[ 0 ]; ++elementIx )
    result[ elementIx ] = std::min( std::max( args[ 0 ][ elementIx ], real( 0 ) ), real( 1 ) );

  return elementsCount[ 0 ];
}
static size_t ValidateSaturate( const EvaluateContext& /*context*/, size_t argCount, const size_t* elementsCount )
{
  if ( argCount != 1 )
    throw ExpressionTree::ValidationError( ErrorType::InvalidArguments, "saturate function takes only one argument with any number of dimensions." );

  return elementsCount[ 0 ];
}

static size_t WrapSin( const EvaluateContext& context, size_t argCount, const size_t* elementsCount, const EvalResult* args, EvalResult result )
{
  assert( argCount == 1 );

  for ( size_t elementIx = 0; elementIx < elementsCount[ 0 ]; ++elementIx )
    result[ elementIx ] = std::sin( CTM( args[ 0 ][ elementIx ] ) );

  return elementsCount[ 0 ];
}
static size_t ValidateSin( const EvaluateContext& /*context*/, size_t argCount, const size_t* elementsCount )
{
  if ( argCount != 1 )
    throw ExpressionTree::ValidationError( ErrorType::InvalidArguments, "sin function takes only one argument with any number of dimensions." );

  return elementsCount[ 0 ];
}

static size_t WrapASin( const EvaluateContext& context, size_t argCount, const size_t* elementsCount, const EvalResult* args, EvalResult result )
{
  assert( argCount == 1 );

  for ( size_t elementIx = 0; elementIx < elementsCount[ 0 ]; ++elementIx )
    result[ elementIx ] = std::asin( MTC( args[ 0 ][ elementIx ] ) );

  return elementsCount[ 0 ];
}
static size_t ValidateASin( const EvaluateContext& /*context*/, size_t argCount, const size_t* elementsCount )
{
  if ( argCount != 1 )
    throw ExpressionTree::ValidationError( ErrorType::InvalidArguments, "asin function takes only one argument with any number of dimensions." );

  return elementsCount[ 0 ];
}

static size_t WrapCos( const EvaluateContext& context, size_t argCount, const size_t* elementsCount, const EvalResult* args, EvalResult result )
{
  assert( argCount == 1 );

  for ( size_t elementIx = 0; elementIx < elementsCount[ 0 ]; ++elementIx )
    result[ elementIx ] = std::cos( CTM( args[ 0 ][ elementIx ] ) );

  return elementsCount[ 0 ];
}
static size_t ValidateCos( const EvaluateContext& /*context*/, size_t argCount, const size_t* elementsCount )
{
  if ( argCount != 1 )
    throw ExpressionTree::ValidationError( ErrorType::InvalidArguments, "cos function takes only one argument with any number of dimensions." );

  return elementsCount[ 0 ];
}

static size_t WrapACos( const EvaluateContext& context, size_t argCount, const size_t* elementsCount, const EvalResult* args, EvalResult result )
{
  assert( argCount == 1 );

  for ( size_t elementIx = 0; elementIx < elementsCount[ 0 ]; ++elementIx )
    result[ elementIx ] = std::acos( MTC( args[ 0 ][ elementIx ] ) );

  return elementsCount[ 0 ];
}
static size_t ValidateACos( const EvaluateContext& /*context*/, size_t argCount, const size_t* elementsCount )
{
  if ( argCount != 1 )
    throw ExpressionTree::ValidationError( ErrorType::InvalidArguments, "acos function takes only one argument with any number of dimensions." );

  return elementsCount[ 0 ];
}

static size_t WrapTan( const EvaluateContext& context, size_t argCount, const size_t* elementsCount, const EvalResult* args, EvalResult result )
{
  assert( argCount == 1 );

  for ( size_t elementIx = 0; elementIx < elementsCount[ 0 ]; ++elementIx )
    result[ elementIx ] = std::tan( CTM( args[ 0 ][ elementIx ] ) );

  return elementsCount[ 0 ];
}
static size_t ValidateTan( const EvaluateContext& /*context*/, size_t argCount, const size_t* elementsCount )
{
  if ( argCount != 1 )
    throw ExpressionTree::ValidationError( ErrorType::InvalidArguments, "tan function takes only one argument with any number of dimensions." );

  return elementsCount[ 0 ];
}

static size_t WrapATan( const EvaluateContext& context, size_t argCount, const size_t* elementsCount, const EvalResult* args, EvalResult result )
{
  assert( argCount == 1 );

  for ( size_t elementIx = 0; elementIx < elementsCount[ 0 ]; ++elementIx )
    result[ elementIx ] = std::atan( MTC( args[ 0 ][ elementIx ] ) );

  return elementsCount[ 0 ];
}
static size_t ValidateATan( const EvaluateContext& /*context*/, size_t argCount, const size_t* elementsCount )
{
  if ( argCount != 1 )
    throw ExpressionTree::ValidationError( ErrorType::InvalidArguments, "atan function takes only one argument with any number of dimensions." );

  return elementsCount[ 0 ];
}

static size_t WrapCtg( const EvaluateContext& context, size_t argCount, const size_t* elementsCount, const EvalResult* args, EvalResult result )
{
  assert( argCount == 1 );

  for ( size_t elementIx = 0; elementIx < elementsCount[ 0 ]; ++elementIx )
    result[ elementIx ] = ctg( CTM( args[ 0 ][ elementIx ] ) );

  return elementsCount[ 0 ];
}
static size_t ValidateCtg( const EvaluateContext& /*context*/, size_t argCount, const size_t* elementsCount )
{
  if ( argCount != 1 )
    throw ExpressionTree::ValidationError( ErrorType::InvalidArguments, "ctg function takes only one argument with any number of dimensions." );

  return elementsCount[ 0 ];
}

static size_t WrapSqrt( const EvaluateContext& context, size_t argCount, const size_t* elementsCount, const EvalResult* args, EvalResult result )
{
  assert( argCount == 1 );

  for ( size_t elementIx = 0; elementIx < elementsCount[ 0 ]; ++elementIx )
    result[ elementIx ] = std::sqrt( args[ 0 ][ elementIx ] );

  return elementsCount[ 0 ];
}
static size_t ValidateSqrt( const EvaluateContext& /*context*/, size_t argCount, const size_t* elementsCount )
{
  if ( argCount != 1 )
    throw ExpressionTree::ValidationError( ErrorType::InvalidArguments, "sqrt function takes only one argument with any number of dimensions." );

  return elementsCount[ 0 ];
}

static size_t MakeVector( const EvaluateContext& context, size_t argCount, const size_t* elementsCount, const EvalResult* args, EvalResult result )
{
  assert( argCount > 1 );

  for ( size_t argIx = 0; argIx < argCount; argIx++ )
  {
    assert( elementsCount[ argIx ] == 1 );
  }

  for ( size_t argIx = 0; argIx < argCount; argIx++ )
    result[ argIx ] = args[ argIx ][ 0 ];

  return argCount;
}
static size_t ValidateMakeVector( const EvaluateContext& /*context*/, size_t argCount, const size_t* elementsCount )
{
  if ( argCount <= 1 )
    throw ExpressionTree::ValidationError( ErrorType::InvalidArguments, "vector function takes two, three or four arguments, each with one dimension." );

  for ( size_t argIx = 0; argIx < argCount; argIx++ )
    if ( elementsCount[ argIx ] != 1 )
      throw ExpressionTree::ValidationError( ErrorType::InvalidDimensions, "vector function takes two, three or four arguments, each with one dimension." );

  return argCount;
}

static size_t VectorLength( const EvaluateContext& context, size_t argCount, const size_t* elementsCount, const EvalResult* args, EvalResult result )
{
  assert( argCount == 1 );
  assert( elementsCount[ 0 ] > 0 );

  real dot = 0;
  for ( size_t elemIx = 0; elemIx < elementsCount[ 0 ]; elemIx++ )
    dot += args[ 0 ][ elemIx ] * args[ 0 ][ elemIx ];

  result[ 0 ] = std::sqrt( dot );

  return 1;
}
static size_t ValidateVectorLength( const EvaluateContext& /*context*/, size_t argCount, const size_t* elementsCount )
{
  if ( argCount != 1 )
    throw ExpressionTree::ValidationError( ErrorType::InvalidArguments, "length function takes only one argument with any number of dimensions." );

  return 1;
}

static size_t VectorNormalize( const EvaluateContext& context, size_t argCount, const size_t* elementsCount, const EvalResult* args, EvalResult result )
{
  assert( argCount == 1 );
  assert( elementsCount[ 0 ] > 0 );

  real length = 0;
  for ( size_t elemIx = 0; elemIx < elementsCount[ 0 ]; elemIx++ )
    length += args[ 0 ][ elemIx ] * args[ 0 ][ elemIx ];

  length = std::sqrt( length );

  if ( length == 0 )
  {
    for ( size_t elemIx = 0; elemIx < elementsCount[ 0 ]; elemIx++ )
      result[ elemIx ] = std::numeric_limits< real >::infinity();
  }
  else
  {
    for ( size_t elemIx = 0; elemIx < elementsCount[ 0 ]; elemIx++ )
      result[ elemIx ] = args[ 0 ][ elemIx ] / length;
  }

  return elementsCount[ 0 ];
}
static size_t ValidateVectorNormalize( const EvaluateContext& /*context*/, size_t argCount, const size_t* elementsCount )
{
  if ( argCount != 1 )
    throw ExpressionTree::ValidationError( ErrorType::InvalidArguments, "normalize function takes only one argument with any number of dimensions." );

  return elementsCount[ 0 ];
}

static size_t VectorDot( const EvaluateContext& context, size_t argCount, const size_t* elementsCount, const EvalResult* args, EvalResult result )
{
  assert( argCount == 2 );
  assert( elementsCount[ 0 ] > 0 );
  assert( elementsCount[ 1 ] > 0 );
  assert( elementsCount[ 0 ] == elementsCount[ 1 ] );

  real dot = 0;
  for ( size_t elemIx = 0; elemIx < elementsCount[ 0 ]; elemIx++ )
    dot += args[ 0 ][ elemIx ] * args[ 1 ][ elemIx ];

  result[ 0 ] = dot;

  return 1;
}
static size_t ValidateVectorDot( const EvaluateContext& /*context*/, size_t argCount, const size_t* elementsCount )
{
  if ( argCount != 2 )
    throw ExpressionTree::ValidationError( ErrorType::InvalidArguments, "dot function takes two arguments with the same number of dimensions." );
  if ( elementsCount[ 0 ] != elementsCount[ 1 ] )
    throw ExpressionTree::ValidationError( ErrorType::InvalidDimensions, "dot function takes two arguments with the same number of dimensions." );

  return 1;
}

static size_t VectorCross( const EvaluateContext& context, size_t argCount, const size_t* elementsCount, const EvalResult* args, EvalResult result )
{
  assert( argCount == 2 );
  assert( elementsCount[ 0 ] == 3 );
  assert( elementsCount[ 1 ] == 3 );

  result[ 0 ] = args[ 0 ][ 1 ] * args[ 1 ][ 2 ] - args[ 0 ][ 2 ] * args[ 1 ][ 1 ];
  result[ 1 ] = args[ 0 ][ 2 ] * args[ 1 ][ 0 ] - args[ 0 ][ 0 ] * args[ 1 ][ 2 ];
  result[ 2 ] = args[ 0 ][ 0 ] * args[ 1 ][ 1 ] - args[ 0 ][ 1 ] * args[ 1 ][ 0 ];

  return 3;
}
static size_t ValidateVectorCross( const EvaluateContext& /*context*/, size_t argCount, const size_t* elementsCount )
{
  if ( argCount != 2 )
    throw ExpressionTree::ValidationError( ErrorType::InvalidArguments, "cross function takes two arguments with three dimensions both." );
  if ( elementsCount[ 0 ] != 3 || elementsCount[ 1 ] != 3 )
    throw ExpressionTree::ValidationError( ErrorType::InvalidDimensions, "cross function takes two arguments with three dimensions both." );

  return 3;
}

static const ExternalFunctions::Function internalFunctions[] =
{
  ExternalFunctions::Function( true, "sin",       WrapSin,         ValidateSin,             1 ),
  ExternalFunctions::Function( true, "cos",       WrapCos,         ValidateCos,             1 ),
  ExternalFunctions::Function( true, "tan",       WrapTan,         ValidateTan,             1 ),
  ExternalFunctions::Function( true, "ctg",       WrapCtg,         ValidateCtg,             1 ),
  ExternalFunctions::Function( true, "asin",      WrapASin,        ValidateASin,            1 ),
  ExternalFunctions::Function( true, "acos",      WrapACos,        ValidateACos,            1 ),
  ExternalFunctions::Function( true, "atan",      WrapATan,        ValidateATan,            1 ),
  ExternalFunctions::Function( true, "sqrt",      WrapSqrt,        ValidateSqrt,            1 ),
  ExternalFunctions::Function( true, "saturate",  WrapSaturate,    ValidateSaturate,        1 ),
  ExternalFunctions::Function( true, "min",       WrapMin,         ValidateMin,             2 ),
  ExternalFunctions::Function( true, "max",       WrapMax,         ValidateMax,             2 ),
  ExternalFunctions::Function( true, "vector",    MakeVector,      ValidateMakeVector,      2 ),
  ExternalFunctions::Function( true, "vector",    MakeVector,      ValidateMakeVector,      3 ),
  ExternalFunctions::Function( true, "vector",    MakeVector,      ValidateMakeVector,      4 ),
  ExternalFunctions::Function( true, "length",    VectorLength,    ValidateVectorLength,    1 ),
  ExternalFunctions::Function( true, "normalize", VectorNormalize, ValidateVectorNormalize, 1 ),
  ExternalFunctions::Function( true, "dot",       VectorDot,       ValidateVectorDot,       2 ),
  ExternalFunctions::Function( true, "cross",     VectorCross,     ValidateVectorCross,     2 ),
};

using ParentMap = std::map< ExpressionTree::Node*, ExpressionTree::Node*, std::less< ExpressionTree::Node* >, STDAllocator< std::pair< const ExpressionTree::Node*, ExpressionTree::Node* > > >;

static ExpressionTree::Node* BuildTree( Tokenizer::Tokens::const_iterator begin
                                      , Tokenizer::Tokens::const_iterator end
                                      , ParentMap& parentMap
                                      , const EvaluateContext& context );

static real ToReal( const char* text, size_t length )
{
  bool afterDot = false;
  real divisor  = real( 0.1 );
  real result   = 0;
  for ( size_t iter = 0; iter < length; ++iter )
  {
    char c = text[ iter ];

    if ( c == '.' )
    {
      afterDot = true;
    }
    else
    {
      real num = real( c - '0' );
      if ( afterDot )
      {
        num     *= divisor;
        result  += num;
        divisor /= 10;
      }
      else
      {
        result *= 10;
        result += num;
      }
    }
  }

  return result;
}

static void ValidateNumber( const char* text, size_t length )
{
  bool dotFound = false;

  for ( size_t iter = 0; iter < length; ++iter )
  {
    char c = text[ iter ];
    if ( c == '.' )
    {
      if ( dotFound )
      {
        std::string errorValue( text, length );
        throw CompileError( ErrorType::SyntaxError, "Malformed number: %s", errorValue.data() );
      }

      dotFound = true;
    }
  }
}

static void Validate( const Tokenizer::Token& token )
{
  switch ( token.type )
  {
  case TokenType::Number:
    ValidateNumber( token.start, token.length );
    break;
  default:
    break;
  }
}

static Tokenizer::Tokens::const_iterator FindClosingParenthesis( Tokenizer::Tokens::const_iterator begin
                                                               , Tokenizer::Tokens::const_iterator end )
{
  int depth = 0;
  for ( auto iter = begin; iter != end; ++iter )
  {
    switch ( iter->type )
    {
    case TokenType::OpenParenthesis:
      ++depth;
      break;
    case TokenType::CloseParenthesis:
      --depth;
      if ( depth == 0 )
        return iter;
      break;
    default:
      break;
    }
  }

  throw CompileError( ErrorType::ParenthezisMismatch, "No closing parenthesis found." );
}

static ExpressionTree::Node* OrderExpression( ExpressionTree::Node* cursor
                                            , ExpressionTree::Node* node
                                            , ExpressionTree::Node* root
                                            , ParentMap& parentMap )
{
  auto nodeOp = static_cast< ExpressionTree::OperatorBase* >( node );

  auto ref = cursor;
  while ( ref )
  {
    auto parentOp = static_cast< ExpressionTree::OperatorBase* >( parentMap[ ref ] );
    if ( parentOp && nodeOp->GetOrder() < parentOp->GetOrder() )
      ref = parentOp;
    else
      return ref;
  }

  return root;
}

static void InsertNode( ExpressionTree::Node*&  cursor
                      , TokenType&              cursorType
                      , ExpressionTree::Node*   newNode
                      , TokenType               newNodeType
                      , ExpressionTree::Node*&  rootNode
                      , ParentMap&              parentMap )
{
  switch ( newNodeType )
  {
  case TokenType::Number:
    switch ( cursorType )
    {
    case TokenType::Unknown:
      // Add the starting number
      rootNode   = newNode;
      cursor     = newNode;
      cursorType = newNodeType;
      break;
    case TokenType::Operator:
      // Add a number after an operator
      static_cast< ExpressionTree::OperatorBase* >( cursor )->SetRightOperand( newNode );
      parentMap[ newNode ] = cursor;
      cursor     = newNode;
      cursorType = newNodeType;
      break;
    default:
      throw CompileError( ErrorType::SyntaxError, "Number appears on an invalid place." );
    }
    break;
  case TokenType::Operator:
    switch ( cursorType )
    {
    case TokenType::Number:
    {
      // Add an operator after a number
      auto insertTo       = OrderExpression( cursor, newNode, rootNode, parentMap );
      auto insertToParent = parentMap[ insertTo ];

      static_cast< ExpressionTree::OperatorBase* >( newNode )->SetLeftOperand( insertTo );
      parentMap[ insertTo ] = newNode;
      parentMap[ newNode  ] = insertToParent;
      if ( insertToParent )
        static_cast< ExpressionTree::OperatorBase* >( insertToParent )->SetRightOperand( newNode );

      if ( rootNode == insertTo )
        rootNode = newNode;
      cursor     = newNode;
      cursorType = newNodeType;
      break;
    }
    case TokenType::Unknown:
    {
      // Negation operator
      auto insertTo       = OrderExpression( cursor, newNode, rootNode, parentMap );
      auto insertToParent = parentMap[ insertTo ];

      static_cast< ExpressionTree::OperatorBase* >( newNode )->SetLeftOperand( insertTo );
      parentMap[ insertTo ] = newNode;
      parentMap[ newNode  ] = insertToParent;
      if ( insertToParent )
        static_cast< ExpressionTree::OperatorBase* >( insertToParent )->SetRightOperand( newNode );

      if ( rootNode == insertTo )
        rootNode = newNode;
      cursor     = newNode;
      cursorType = newNodeType;
      break;
    }
    default:
      throw CompileError( ErrorType::SyntaxError, "Operator appears on an invalid place." );
    }
    break;
  default:
    throw CompileError( ErrorType::SyntaxError, "Invalid token." );
  }
}

static bool NameCompare( const char* ref, size_t refSize, const char* val, size_t valSize )
{
  if ( refSize != valSize )
    return false;

  for ( size_t iter = 0; iter < valSize && ref[ iter ]; ++iter )
    if ( std::tolower( ref[ iter ] ) != std::tolower( val[ iter ] ) )
      return false;

  return true;
}

static ExpressionTree::Node* CreateFunction( Tokenizer::Tokens::const_iterator& begin
                                           , Tokenizer::Tokens::const_iterator  end
                                           , ParentMap&                         parentMap
                                           , const EvaluateContext&             context
                                           , const ExternalFunctions::Function& func )
{
  auto funcOpen = begin + 1;

  if ( funcOpen->type != TokenType::OpenParenthesis )
  {
    std::string wrongToken( begin->start, begin->length );
    throw CompileError( ErrorType::SyntaxError, "Function name should be followed by an opening parenthesis: %s", wrongToken.data() );
  }

  auto closing = FindClosingParenthesis( funcOpen, end );

  if ( func.argCount == 0 && ( end - funcOpen ) != 1 )
    return nullptr;

  if ( func.argCount == 0 )
  {
    begin = closing;
    return new ExpressionTree::Function< 0 >( func );
  }
  else
  {
    using ArgSection = std::pair< Tokenizer::Tokens::const_iterator, Tokenizer::Tokens::const_iterator >;
    std::unique_ptr< ArgSection[] > argSections( new ArgSection[ func.argCount ] );

    std::unique_ptr< ExpressionTree::FunctionBase > resultFunction;
    switch ( func.argCount )
    {
    case 1:
      resultFunction.reset( new ExpressionTree::Function< 1 >( func ) );
      break;
    case 2:
      resultFunction.reset( new ExpressionTree::Function< 2 >( func ) );
      break;
    case 3:
      resultFunction.reset( new ExpressionTree::Function< 3 >( func ) );
      break;
    case 4:
      resultFunction.reset( new ExpressionTree::Function< 4 >( func ) );
      break;
    case 5:
      resultFunction.reset( new ExpressionTree::Function< 5 >( func ) );
      break;
    case 6:
      resultFunction.reset( new ExpressionTree::Function< 6 >( func ) );
      break;
    case 7:
      resultFunction.reset( new ExpressionTree::Function< 7 >( func ) );
      break;
    case 8:
      resultFunction.reset( new ExpressionTree::Function< 8 >( func ) );
      break;
    }

    bool   newSection     = true;
    size_t currentSection = 0;
    auto   iter           = funcOpen + 1;
    for ( ; iter != closing && currentSection < func.argCount; ++iter )
    {
      if ( iter->type == TokenType::Comma )
      {
        if ( newSection )
          return nullptr;

        argSections[ currentSection ].second = iter;
        ++currentSection;
        newSection = true;
      }
      else if ( newSection )
      {
        argSections[ currentSection ].first = iter;
        newSection                          = false;
      }

      if ( iter->type == TokenType::OpenParenthesis )
        iter = FindClosingParenthesis( iter, closing );
    }

    if ( newSection && currentSection < func.argCount )
      return nullptr;

    if ( iter != closing || currentSection != func.argCount - 1 )
      return nullptr;

    argSections[ currentSection ].second = iter;

    for ( size_t argIndex = 0; argIndex < func.argCount; ++argIndex )
      resultFunction->SetOperand( argIndex, BuildTree( argSections[ argIndex ].first
                                                     , argSections[ argIndex ].second
                                                     , parentMap
                                                     , context ) );

    begin = closing;
    return resultFunction.release();
  }
}

static ExpressionTree::Node* CreateFromString( Tokenizer::Tokens::const_iterator& begin
                                             , Tokenizer::Tokens::const_iterator  end
                                             , ParentMap&                         parentMap
                                             , const EvaluateContext&             context )
{
  for ( size_t icIx = 0; icIx < internalConstants.instanceCount; ++icIx )
  {
    const auto& ic = internalConstants.instances[ icIx ];
    if ( NameCompare( ic.name, ic.nameLength, begin->start, begin->length ) )
    {
      switch ( internalConstants.instances[ icIx ].dimensions )
      {
      case 1:
        return new ExpressionTree::ConstantValue< 1 >( internalConstants.instances[ icIx ].value );
      case 2:
        return new ExpressionTree::ConstantValue< 2 >( internalConstants.instances[ icIx ].value );
      case 3:
        return new ExpressionTree::ConstantValue< 3 >( internalConstants.instances[ icIx ].value );
      case 4:
        return new ExpressionTree::ConstantValue< 4 >( internalConstants.instances[ icIx ].value );
      default:
        assert( false && "Invalid internal variable dimensions." );
        return nullptr;
      }
    }
  }

  for ( size_t index = 0; index < context.variables.instanceCount; ++index )
  {
    const auto& ec = context.variables.instances[ index ];
    if ( NameCompare( ec.name, ec.nameLength, begin->start, begin->length ) )
    {
      if ( context.variables.instances[ index ].isConst )
      {
        switch ( context.variables.instances[ index ].dimensions )
        {
        case 1:
          return new ExpressionTree::ConstantValue< 1 >( context.variables.instances[ index ].value );
        case 2:
          return new ExpressionTree::ConstantValue< 2 >( context.variables.instances[ index ].value );
        case 3:
          return new ExpressionTree::ConstantValue< 3 >( context.variables.instances[ index ].value );
        case 4:
          return new ExpressionTree::ConstantValue< 4 >( context.variables.instances[ index ].value );
        default:
          assert( false && "Invalid internal variable dimensions." );
          return nullptr;
        }
      }
      else
      {
        switch ( context.variables.instances[ index ].dimensions )
        {
        case 1:
          return new ExpressionTree::VariableValue< 1 >( index );
        case 2:
          return new ExpressionTree::VariableValue< 2 >( index );
        case 3:
          return new ExpressionTree::VariableValue< 3 >( index );
        case 4:
          return new ExpressionTree::VariableValue< 4 >( index );
        default:
          assert( false && "Invalid external variable dimensions." );
          return nullptr;
        }
      }
    }
  }

  bool hasFunctionNameMatch = false;
  for ( const auto& ifn : internalFunctions )
  {
    if ( NameCompare( ifn.name, ifn.nameLength, begin->start, begin->length ) )
    {
      hasFunctionNameMatch = true;
      if ( auto func = CreateFunction( begin, end, parentMap, context, ifn ) )
        return func;
    }
  }

  for ( size_t fnIx = 0; fnIx < context.functions.instanceCount; ++fnIx )
  {
    const auto& xfn = context.functions.instances[ fnIx ];
    if ( NameCompare( xfn.name, xfn.nameLength, begin->start, begin->length ) )
    {
      hasFunctionNameMatch = true;
      if ( auto func = CreateFunction( begin, end, parentMap, context, xfn ) )
        return func;
    }
  }

  std::string wrongToken( begin->start, begin->length );
  if ( hasFunctionNameMatch )
    throw CompileError( ErrorType::InvalidArguments, "Can't match function because of wrong number of arguments: %s", wrongToken.data() );
  else
    throw CompileError( ErrorType::SyntaxError, "Unknown string: %s", wrongToken.data() );
}

static ExpressionTree::Node* BuildTree( Tokenizer::Tokens::const_iterator begin
                                      , Tokenizer::Tokens::const_iterator end
                                      , ParentMap& parentMap
                                      , const EvaluateContext& context )
{
  ExpressionTree::Node* root        = nullptr;
  ExpressionTree::Node* cursor      = nullptr;
  TokenType             cursorToken = TokenType::Unknown;

  for ( auto iter = begin; iter != end; ++iter )
  {
    switch ( iter->type )
    {
    case TokenType::Number:
    {
      auto number = real( ToReal( iter->start, iter->length ) );
      if ( cursorToken != TokenType::Operator && cursorToken != TokenType::Unknown )
        throw CompileError( ErrorType::SyntaxError, "Unexpected number: %f", number );
      auto node = new ExpressionTree::ConstantValue< 1 >( &number );
      InsertNode( cursor, cursorToken, node, iter->type, root, parentMap );
      break;
    }
    case TokenType::Operator:
    {
      char opc = iter->start[ 0 ];
      bool isNegation = opc == '-' && cursorToken == TokenType::Unknown;
      if ( cursorToken != TokenType::Number && !isNegation )
        throw CompileError( ErrorType::SyntaxError, "Unexpected operator: %c", opc );
      ExpressionTree::Node* node = nullptr;
      switch ( opc )
      {
      case '+':
        node = new ExpressionTree::OperatorAdd;
        break;
      case '-':
        if ( cursorToken == TokenType::Unknown )
          node = new ExpressionTree::OperatorNegate;
        else
          node = new ExpressionTree::OperatorSub;
        break;
      case '*':
        node = new ExpressionTree::OperatorMul;
        break;
      case '/':
        node = new ExpressionTree::OperatorDiv;
        break;
      case '^':
        node = new ExpressionTree::OperatorPow;
        break;
      case '%':
        node = new ExpressionTree::OperatorMod;
        break;
      default:
        throw CompileError( ErrorType::SyntaxError, "Unknown operator: %c", opc );
      }
      InsertNode( cursor, cursorToken, node, iter->type, root, parentMap );
      break;
    }
    case TokenType::OpenParenthesis:
    {
      auto closingToken = FindClosingParenthesis( iter, end );

      ParentMap closedParentMap;
      auto node = BuildTree( iter + 1, closingToken, closedParentMap, context );

      InsertNode( cursor, cursorToken, node, TokenType::Number, root, parentMap );

      iter = closingToken;

      break;
    }
    case TokenType::String:
    {
      if ( cursorToken != TokenType::Operator && cursorToken != TokenType::Unknown )
      {
        std::string wrongToken( iter->start, iter->length );
        throw CompileError( ErrorType::SyntaxError, "Unexpected string: %s", wrongToken.data() );
      }
      auto node = CreateFromString( iter, end, parentMap, context );
      InsertNode( cursor, cursorToken, node, TokenType::Number, root, parentMap );
      break;
    }
    default:
    {
      std::string wrongToken( iter->start, iter->length );
      throw CompileError( ErrorType::SyntaxError, "Unknown token: %s", wrongToken.data() );
    }
    }
  }

  return root;
}

ExpressionTree::Node* Parser::Parse( const char* expressionText
                                   , size_t length
                                   , ParseErrorDetails& error
                                   , const EvaluateContext& context )
{
  try
  {
		if ( !expressionText || length == 0 )
			throw ExpressionTree::ValidationError( ErrorType::InternalError, "Expression is zero length or null pointer." );

    Tokenizer tokenizer( expressionText, length );

    for ( const auto& token : tokenizer.GetTokens() )
      Validate( token );

    ParentMap parentMap;
    auto tree = BuildTree( tokenizer.GetTokens().begin(), tokenizer.GetTokens().end(), parentMap, context );
    tree->Validate( context );
    tree = ExpressionTree::Optimize( tree, context );
    return tree;
  }
  catch ( const Exception& e )
  {
    auto length = std::min( sizeof( error.details ) - 1, std::strlen( e.what() ) );
    std::memcpy( error.details, e.what(), length );
    
    error.type              = e.GetErrorType();
    error.details[ length ] = 0;
    
    return nullptr;
  }
  catch ( const std::exception& e )
  {
    auto length = std::min( sizeof( error.details ) - 1, std::strlen( e.what() ) );
    std::memcpy( error.details, e.what(), length );

    error.type              = ErrorType::UnknownError;
    error.details[ length ] = 0;

    return nullptr;
  }
}

}
}
