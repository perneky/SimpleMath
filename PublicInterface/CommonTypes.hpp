#pragma once

#include <cstddef>

#ifdef _MSC_VER
# ifdef SM_DLL
#  define SM_DLLIFACE __declspec( dllexport )
# else
#  define SM_DLLIFACE __declspec( dllimport )
# endif // SM_DLL
#elif defined __GNUC__
# ifdef SM_DLL
#  define SM_DLLIFACE __attribute__ ((visibility("default")))
# else
#  define SM_DLLIFACE
# endif // SM_DLL
#endif // SM_DLLIFACE

namespace SimpleMath
{

#if USE_DOUBLE_PRECISION
using real = double;
#else
using real = float;
#endif // USE_DOUBLE_PRECISION

using size_t = std::size_t;

using EvalResult = real[ 4 ];

}