#include "Parser.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstring>

#include "ExpressionTree/ConstantValue.hpp"
#include "ExpressionTree/OperatorAdd.hpp"
#include "ExpressionTree/OperatorSub.hpp"
#include "ExpressionTree/OperatorMul.hpp"
#include "ExpressionTree/OperatorDiv.hpp"
#include "ExpressionTree/OperatorPow.hpp"
#include "ExpressionTree/OperatorMod.hpp"
#include "ExpressionTree/Function.hpp"
#include "Tokenizer.hpp"
#include "CompileError.hpp"

namespace SimpleMath
{
namespace Parser
{

static const real piValue     = real( 3.14159265358979323846 );
static const real halfPiValue = real( 1.57079632679489661923 );

static real ToRadians( real x )
{
  return x * piValue / 180;
}

static real ToDegrees( real x )
{
  return x * 180 / piValue;
}

#if USE_DEGREES_FOR_ANGLES
# define CTM( x ) ToRadians( (x) )
# define MTC( x ) ToDegrees( (x) )
#else
# define CTM( x ) (x)
# define MTC( x ) (x)
#endif // USE_DEGREES_FOR_ANGLES

static const ExternalVariables::Variable internalVariableList[] =
{
  ExternalVariables::Variable( "pi",        piValue     ),
  ExternalVariables::Variable( "two_pi",    piValue * 2 ),
  ExternalVariables::Variable( "half_pi",   halfPiValue ),

  ExternalVariables::Variable( "vec2_zero",  0, 0 ),
  ExternalVariables::Variable( "vec2_one",   1, 1 ),
  ExternalVariables::Variable( "vec2_xaxis", 1, 0 ),
  ExternalVariables::Variable( "vec2_yaxis", 0, 1 ),

  ExternalVariables::Variable( "vec3_zero",  0, 0, 0 ),
  ExternalVariables::Variable( "vec3_one",   1, 1, 1 ),
  ExternalVariables::Variable( "vec3_xaxis", 1, 0, 0 ),
  ExternalVariables::Variable( "vec3_yaxis", 0, 1, 0 ),
  ExternalVariables::Variable( "vec3_zaxis", 0, 0, 1 ),

  ExternalVariables::Variable( "vec4_zero",  0, 0, 0, 0 ),
  ExternalVariables::Variable( "vec4_one",   1, 1, 1, 1 ),
  ExternalVariables::Variable( "vec4_xaxis", 1, 0, 0, 0 ),
  ExternalVariables::Variable( "vec4_yaxis", 0, 1, 0, 0 ),
  ExternalVariables::Variable( "vec4_zaxis", 0, 0, 1, 0 ),
  ExternalVariables::Variable( "vec4_waxis", 0, 0, 0, 1 ),
};

static const ExternalVariables internalConstants( internalVariableList );

static size_t WrapMin( const EvaluateContext& context, size_t argCount, const size_t* elementsCount, const EvalResult* args, EvalResult result )
{
  assert( argCount == 2 );
  if ( argCount != 2 )
    return 0;

  assert( elementsCount[ 0 ] == elementsCount[ 1 ] );
  if ( elementsCount[ 0 ] != elementsCount[ 1 ] )
    return 0;

  for ( size_t elementIx = 0; elementIx < elementsCount[ 0 ]; ++elementIx )
    result[ elementIx ] = std::min( args[ 0 ][ elementIx ], args[ 1 ][ elementIx ] );

  return elementsCount[ 0 ];
}

static size_t WrapMax( const EvaluateContext& context, size_t argCount, const size_t* elementsCount, const EvalResult* args, EvalResult result )
{
  assert( argCount == 2 );
  if ( argCount != 2 )
    return 0;

  assert( elementsCount[ 0 ] == elementsCount[ 1 ] );
  if ( elementsCount[ 0 ] != elementsCount[ 1 ] )
    return 0;

  for ( size_t elementIx = 0; elementIx < elementsCount[ 0 ]; ++elementIx )
    result[ elementIx ] = std::max( args[ 0 ][ elementIx ], args[ 1 ][ elementIx ] );

  return elementsCount[ 0 ];
}

static size_t WrapSaturate( const EvaluateContext& context, size_t argCount, const size_t* elementsCount, const EvalResult* args, EvalResult result )
{
  assert( argCount == 1 );
  if ( argCount != 1 )
    return 0;

  for ( size_t elementIx = 0; elementIx < elementsCount[ 0 ]; ++elementIx )
    result[ elementIx ] = std::min( std::max( args[ 0 ][ elementIx ], real( 0 ) ), real( 1 ) );

  return elementsCount[ 0 ];
}

static size_t WrapSin( const EvaluateContext& context, size_t argCount, const size_t* elementsCount, const EvalResult* args, EvalResult result )
{
  assert( argCount == 1 );
  if ( argCount != 1 )
    return 0;

  for ( size_t elementIx = 0; elementIx < elementsCount[ 0 ]; ++elementIx )
    result[ elementIx ] = std::sin( CTM( args[ 0 ][ elementIx ] ) );

  return elementsCount[ 0 ];
}
static size_t WrapASin( const EvaluateContext& context, size_t argCount, const size_t* elementsCount, const EvalResult* args, EvalResult result )
{
  assert( argCount == 1 );
  if ( argCount != 1 )
    return 0;

  for ( size_t elementIx = 0; elementIx < elementsCount[ 0 ]; ++elementIx )
    result[ elementIx ] = std::asin( MTC( args[ 0 ][ elementIx ] ) );

  return elementsCount[ 0 ];
}

static size_t WrapCos( const EvaluateContext& context, size_t argCount, const size_t* elementsCount, const EvalResult* args, EvalResult result )
{
  assert( argCount == 1 );
  if ( argCount != 1 )
    return 0;

  for ( size_t elementIx = 0; elementIx < elementsCount[ 0 ]; ++elementIx )
    result[ elementIx ] = std::cos( CTM( args[ 0 ][ elementIx ] ) );

  return elementsCount[ 0 ];
}
static size_t WrapACos( const EvaluateContext& context, size_t argCount, const size_t* elementsCount, const EvalResult* args, EvalResult result )
{
  assert( argCount == 1 );
  if ( argCount != 1 )
    return 0;

  for ( size_t elementIx = 0; elementIx < elementsCount[ 0 ]; ++elementIx )
    result[ elementIx ] = std::acos( MTC( args[ 0 ][ elementIx ] ) );

  return elementsCount[ 0 ];
}

static size_t WrapTan( const EvaluateContext& context, size_t argCount, const size_t* elementsCount, const EvalResult* args, EvalResult result )
{
  assert( argCount == 1 );
  if ( argCount != 1 )
    return 0;

  for ( size_t elementIx = 0; elementIx < elementsCount[ 0 ]; ++elementIx )
    result[ elementIx ] = std::tan( CTM( args[ 0 ][ elementIx ] ) );

  return elementsCount[ 0 ];
}
static size_t WrapATan( const EvaluateContext& context, size_t argCount, const size_t* elementsCount, const EvalResult* args, EvalResult result )
{
  assert( argCount == 1 );
  if ( argCount != 1 )
    return 0;

  for ( size_t elementIx = 0; elementIx < elementsCount[ 0 ]; ++elementIx )
    result[ elementIx ] = std::atan( MTC( args[ 0 ][ elementIx ] ) );

  return elementsCount[ 0 ];
}

static size_t WrapCtg( const EvaluateContext& context, size_t argCount, const size_t* elementsCount, const EvalResult* args, EvalResult result )
{
  assert( argCount == 1 );
  if ( argCount != 1 )
    return 0;

  for ( size_t elementIx = 0; elementIx < elementsCount[ 0 ]; ++elementIx )
    result[ elementIx ] = std::tan( halfPiValue - CTM( args[ 0 ][ elementIx ] ) );

  return elementsCount[ 0 ];
}

static size_t WrapSqrt( const EvaluateContext& context, size_t argCount, const size_t* elementsCount, const EvalResult* args, EvalResult result )
{
  assert( argCount == 1 );
  if ( argCount != 1 )
    return 0;

  for ( size_t elementIx = 0; elementIx < elementsCount[ 0 ]; ++elementIx )
    result[ elementIx ] = std::sqrt( args[ 0 ][ elementIx ] );

  return elementsCount[ 0 ];
}

static size_t MakeVector( const EvaluateContext& context, size_t argCount, const size_t* elementsCount, const EvalResult* args, EvalResult result )
{
  assert( argCount > 1 );
  if ( argCount <= 1 )
    return 0;

  for ( size_t argIx = 0; argIx < argCount; argIx++ )
  {
    assert( elementsCount[ argIx ] == 1 );
    if ( elementsCount[ argIx ] != 1 )
      return 0;
  }

  for ( size_t argIx = 0; argIx < argCount; argIx++ )
    result[ argIx ] = args[ argIx ][ 0 ];

  return argCount;
}

static size_t VectorLength( const EvaluateContext& context, size_t argCount, const size_t* elementsCount, const EvalResult* args, EvalResult result )
{
  assert( argCount == 1 );
  if ( argCount != 1 )
    return 0;

  assert( elementsCount[ 0 ] > 0 );
  if ( elementsCount[ 0 ] == 0 )
    return 0;

  real dot = 0;
  for ( size_t elemIx = 0; elemIx < elementsCount[ 0 ]; elemIx++ )
    dot += args[ 0 ][ elemIx ] * args[ 0 ][ elemIx ];

  result[ 0 ] = std::sqrt( dot );

  return 1;
}

static size_t VectorNormalize( const EvaluateContext& context, size_t argCount, const size_t* elementsCount, const EvalResult* args, EvalResult result )
{
  assert( argCount == 1 );
  if ( argCount != 1 )
    return 0;

  assert( elementsCount[ 0 ] > 0 );
  if ( elementsCount[ 0 ] == 0 )
    return 0;

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

static size_t VectorDot( const EvaluateContext& context, size_t argCount, const size_t* elementsCount, const EvalResult* args, EvalResult result )
{
  assert( argCount == 2 );
  if ( argCount != 2 )
    return 0;

  assert( elementsCount[ 0 ] > 0 );
  if ( elementsCount[ 0 ] == 0 )
    return 0;

  assert( elementsCount[ 1 ] > 0 );
  if ( elementsCount[ 1 ] == 0 )
    return 0;

  assert( elementsCount[ 0 ] == elementsCount[ 1 ] );
  if ( elementsCount[ 0 ] != elementsCount[ 1 ] )
    return 0;

  real dot = 0;
  for ( size_t elemIx = 0; elemIx < elementsCount[ 0 ]; elemIx++ )
    dot += args[ 0 ][ elemIx ] * args[ 1 ][ elemIx ];

  result[ 0 ] = dot;

  return 1;
}

static size_t VectorCross( const EvaluateContext& context, size_t argCount, const size_t* elementsCount, const EvalResult* args, EvalResult result )
{
  assert( argCount == 2 );
  if ( argCount != 2 )
    return 0;

  assert( elementsCount[ 0 ] == 3 );
  if ( elementsCount[ 0 ] != 3 )
    return 0;

  assert( elementsCount[ 1 ] == 3 );
  if ( elementsCount[ 1 ] != 3 )
    return 0;

  result[ 0 ] = args[ 0 ][ 1 ] * args[ 1 ][ 2 ] - args[ 0 ][ 2 ] * args[ 1 ][ 1 ];
  result[ 1 ] = args[ 0 ][ 2 ] * args[ 1 ][ 0 ] - args[ 0 ][ 0 ] * args[ 1 ][ 2 ];
  result[ 2 ] = args[ 0 ][ 0 ] * args[ 1 ][ 1 ] - args[ 0 ][ 1 ] * args[ 1 ][ 0 ];

  return 3;
}

static const ExternalFunctions::Function internalFunctions[] =
{
  ExternalFunctions::Function( "sin",       WrapSin,         1 ),
  ExternalFunctions::Function( "cos",       WrapCos,         1 ),
  ExternalFunctions::Function( "tan",       WrapTan,         1 ),
  ExternalFunctions::Function( "ctg",       WrapCtg,         1 ),
  ExternalFunctions::Function( "asin",      WrapASin,        1 ),
  ExternalFunctions::Function( "acos",      WrapACos,        1 ),
  ExternalFunctions::Function( "atan",      WrapATan,        1 ),
  ExternalFunctions::Function( "sqrt",      WrapSqrt,        1 ),
  ExternalFunctions::Function( "saturate",  WrapSaturate,    1 ),
  ExternalFunctions::Function( "min",       WrapMin,         2 ),
  ExternalFunctions::Function( "max",       WrapMax,         2 ),
  ExternalFunctions::Function( "vector",    MakeVector,      2 ),
  ExternalFunctions::Function( "vector",    MakeVector,      3 ),
  ExternalFunctions::Function( "vector",    MakeVector,      4 ),
  ExternalFunctions::Function( "length",    VectorLength,    1 ),
  ExternalFunctions::Function( "normalize", VectorNormalize, 1 ),
  ExternalFunctions::Function( "dot",       VectorDot,       2 ),
  ExternalFunctions::Function( "cross",     VectorCross,     2 ),
};

using ParentMap = std::map< ExpressionTree::Node*, ExpressionTree::Node* >;

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
        throw CompileError( "Malformed number: %s", errorValue.data() );
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
  }
}

static void Check( bool condition, const char* format, ... )
{
  if ( !condition )
  {
    va_list args;
    va_start( args, format );

    OnScopeExit( va_end( args ) );

    throw CompileError( format, args );
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
    }
  }

  throw CompileError( "No closing parenthesis found." );
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
      throw std::exception();
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
    default:
      throw std::exception();
    }
    break;
  default:
    throw std::exception();
  }
}

static bool NameCompare( const char* ref, const char* val, size_t valSize )
{
  for ( size_t iter = 0; iter < valSize && ref[ iter ]; ++iter )
    if ( tolower( ref[ iter ] ) != tolower( val[ iter ] ) )
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
    throw CompileError( "Function name should be followed by an opening parenthesis: %s", wrongToken.data() );
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
    std::vector< ArgSection > argSections( func.argCount );

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
    for ( ; iter != closing && currentSection < argSections.size(); ++iter )
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

    if ( newSection && currentSection < argSections.size() )
      return nullptr;

    if ( iter != closing || currentSection != argSections.size() - 1 )
      return nullptr;

    argSections[ currentSection ].second = iter;

    for ( size_t argIndex = 0; argIndex < argSections.size(); ++argIndex )
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
    if ( NameCompare( internalConstants.instances[ icIx ].name, begin->start, begin->length ) )
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
    if ( NameCompare( context.variables.instances[ index ].name, begin->start, begin->length ) )
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
        assert( false && "Invalid external variable dimensions." );
        return nullptr;
      }
    }
  }

  for ( const auto& ifn : internalFunctions )
    if ( NameCompare( ifn.name, begin->start, begin->length ) )
      if ( auto func = CreateFunction( begin, end, parentMap, context, ifn ) )
        return func;

  std::string wrongToken( begin->start, begin->length );
  throw CompileError( "Unknown string, or can't match function: %s", wrongToken.data() );
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
      Check( cursorToken == TokenType::Operator || cursorToken == TokenType::Unknown, "Unexpected number: %f", number );
      auto node = new ExpressionTree::ConstantValue< 1 >( &number );
      InsertNode( cursor, cursorToken, node, iter->type, root, parentMap );
      break;
    }
    case TokenType::Operator:
    {
      char opc = iter->start[ 0 ];
      if ( cursorToken == TokenType::Unknown && opc == '-' )
      {
        // Negation at the start of an expression
        real zero = 0;
        auto node = new ExpressionTree::ConstantValue< 1 >( &zero );
        InsertNode( cursor, cursorToken, node, TokenType::Number, root, parentMap );
      }
      Check( cursorToken == TokenType::Number, "Unexpected operator: %c", opc );
      ExpressionTree::Node* node = nullptr;
      switch ( opc )
      {
      case '+':
        node = new ExpressionTree::OperatorAdd;
        break;
      case '-':
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
        throw CompileError( "Unknown operator: %c", opc );
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
        throw CompileError( "Unexpected string: %s", wrongToken.data() );
      }
      auto node = CreateFromString( iter, end, parentMap, context );
      InsertNode( cursor, cursorToken, node, TokenType::Number, root, parentMap );
      break;
    }
    default:
    {
      std::string wrongToken( iter->start, iter->length );
      throw CompileError( "Unknown token: %s", wrongToken.data() );
    }
    }
  }

  return root;
}

ExpressionTree::Node::Unique Parser::Parse( const char* expressionText
                                          , size_t length
                                          , char* errorBuffer
                                          , size_t errorBufferSize
                                          , const EvaluateContext& context )
{
  try
  {
    Tokenizer tokenizer( expressionText, length );

    for ( const auto& token : tokenizer.GetTokens() )
      Validate( token );

    ParentMap parentMap;
    return ExpressionTree::Node::Unique( BuildTree( tokenizer.GetTokens().begin(), tokenizer.GetTokens().end(), parentMap, context ) );
  }
  catch ( const std::exception& e )
  {
    auto length = std::min( errorBufferSize - 1, std::strlen( e.what() ) );
    std::memcpy( errorBuffer, e.what(), length );
    errorBuffer[ length ] = 0;
    return nullptr;
  }
}

}
}
