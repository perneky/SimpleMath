#include "TestBase.hpp"

struct ValidationTesting : ResultTesting {};

TEST_F( ValidationTesting, empty_string )
{
	TestError( "", ErrorType::InternalError );
}

TEST_F( ValidationTesting, null_string )
{
	TestError( nullptr, ErrorType::InternalError );
}

TEST_F( ValidationTesting, sin_with_zero_args )
{
  TestError( "sin()", ErrorType::InvalidArguments );
}

TEST_F( ValidationTesting, sin_with_two_args )
{
  TestError( "sin( 1, 2 )", ErrorType::InvalidArguments );
}

TEST_F( ValidationTesting, wrong_operators_1 )
{
  TestError( "4++5", ErrorType::SyntaxError );
}

TEST_F( ValidationTesting, wrong_operators_2 )
{
  TestError( "/4*5+", ErrorType::SyntaxError );
}

TEST_F( ValidationTesting, wrong_operators_dimension_problem )
{
  TestError( "vector(2,3)+vector(3,4,5)", ErrorType::InvalidDimensions );
}

TEST_F( ValidationTesting, cross_with_wrong_dimensions )
{
  TestError( "cross(vector(2,3),vector(3,4))", ErrorType::InvalidDimensions );
}

TEST_F( ValidationTesting, dot_with_wrong_dimensions )
{
  TestError( "dot(vector(2,3,4),vector(3,4))", ErrorType::InvalidDimensions );
}

TEST_F( ValidationTesting, invalid_name )
{
  TestError( "dox(5)", ErrorType::SyntaxError );
}

TEST_F( ValidationTesting, number_in_wrong_place )
{
  TestError( "6 6", ErrorType::SyntaxError );
}

TEST_F( ValidationTesting, invalid_token )
{
  TestError( "6@6", ErrorType::SyntaxError );
}

TEST_F( ValidationTesting, parenthezis_1 )
{
  TestError( "sin(5", ErrorType::ParenthezisMismatch );
}

TEST_F( ValidationTesting, parenthezis_2 )
{
  TestError( "(5+6", ErrorType::ParenthezisMismatch );
}
