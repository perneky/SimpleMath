#pragma once

#if USE_DOUBLE_PRECISION
using real = double;
#else
using real = float;
#endif // USE_DOUBLE_PRECISION

#ifdef _MSC_VER
# ifdef SM_DLL
#  define DLLIFACE __declspec( dllexport )
# else
#  define DLLIFACE __declspec( dllimport )
# endif // SM_DLL
#elif defined __GNUC__
# ifdef SM_DLL
#  define DLLIFACE __attribute__ ((visibility("default")))
# else
#  define DLLIFACE
# endif // SM_DLL
#endif // DLLIFACE

#include <cstddef>
using size_t = std::size_t;

using EvalResult = real[ 4 ];
