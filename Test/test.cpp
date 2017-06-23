#include <cstring>
#include "../Submodules/gtest/googletest/include/gtest/gtest.h"
#include "SimpleMath.hpp"

#ifdef WIN32
# include "Windows.h"
#endif // WIN32

int main( int argc, char* argv[] )
{
  testing::InitGoogleTest( &argc, argv );

  auto result = RUN_ALL_TESTS();

#ifdef WIN32
  if ( IsDebuggerPresent() )
    std::getchar();
#endif // WIN32

  return result;
}