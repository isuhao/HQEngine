/*
Copyright (C) 2010-2013  Le Hoang Quyen (lehoangq@gmail.com)

This program is free software; you can redistribute it and/or
modify it under the terms of the MIT license.  See the file
COPYING.txt included with this distribution for more information.


*/

// stdafx.cpp : source file that includes just the standard includes
// HQUtilMath.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "HQUtilMathPCH.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

#ifdef HQ_SSE_MATH
#	pragma message("using SSE Math")
#elif defined HQ_NEON_MATH
#	pragma message("using Neon Math")
#	if defined HQ_NEON_ASM
#		pragma message("using Neon ASM")
#	endif
#elif defined HQ_DIRECTX_MATH
#	pragma message("using DirectX Math")
#elif defined HQ_CMATH
#	pragma message("using C Math")
#endif
