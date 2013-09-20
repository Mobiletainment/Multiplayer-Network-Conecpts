//!---------------------------------------------------------------------------
//! \file "slMathTemplates.h"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2012-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------


#ifndef __SLMATHTEMPLATES_H__
#define __SLMATHTEMPLATES_H__

//-----------------------------------------------------------------------------
// Copyright (c) 2011, Jan Fietz, Cyrus Preuss
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification, 
// are permitted provided that the following conditions are met:
// 
// * Redistributions of source code must retain the above copyright notice, 
//   this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice, 
//   this list of conditions and the following disclaimer in the documentation 
//   and/or other materials provided with the distribution.
// * Neither the name of EduNetGames nor the names of its contributors
//   may be used to endorse or promote products derived from this software
//   without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON 
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#include <math.h>

namespace sl	{


	//-----------------------------------------------------------------------------
	template <class T>
	SL_FORCEINLINE
		const T& min(const T& a, const T& b) 
	{
		return a < b ? a : b ;
	}

	//-----------------------------------------------------------------------------
	template <class T>
	SL_FORCEINLINE
		const T& max(const T& a, const T& b) 
	{
		return  a > b ? a : b;
	}

	//-----------------------------------------------------------------------------
	template <class T>
	SL_FORCEINLINE
		T clamp(const T& fValue, const T& fMin, const T& fMax) {
			assert( fMin <= fMax );
			T fValueOut = sl::max( fValue, fMin );
			fValueOut = sl::min( fValueOut, fMax );
			return fValueOut;
	}

	//-----------------------------------------------------------------------------
	template <class T>
	SL_FORCEINLINE
		T clampSave(const T& fValue, const T& fMin, const T& fMax) {
			T _fMin = sl::min( fMin, fMax );
			T _fMax = sl::max( fMin, fMax );
			T fValueOut = etMax( fValue, _fMin );
			fValueOut = etMin( fValueOut, _fMax );
			return fValueOut;
	}

	//-----------------------------------------------------------------------------
	template <class T>
	SL_FORCEINLINE
		T sign(const T& fValue)
	{
		return ( fValue > T( 0.0 ) ? T( +1.0 ) : ( fValue < T( 0.0 ) ? T( -1.0 ) : T( 0.0 ) ) );
	}	

	//-----------------------------------------------------------------------------
	template <class T>
	SL_FORCEINLINE
		T interval(const T& fMin, const T& fMax)
	{
		assert( fMin <= fMax );
		return ( fMax - fMin );
	}	

	//-----------------------------------------------------------------------------
	//! @brief blends from a to b with rt
	//! @return the linear interpolation between a and b vector 
	//!  * @param a The source 
	//!  * @param b The target 
	//!  * @param rt The ration of a to b (rt = 0 => return a, rt=1 => return b)
	template <class T>
	SL_FORCEINLINE
		const T getInterpolate(const T& a, const T& b, T rt) 
	{
		const T maxValue(1);
		const T minValue(0);
		rt = max(minValue, min(rt, maxValue));
		T s = maxValue - rt;
		return (s * a + rt * b);
	}


}




#endif // __SLMATHTEMPLATES_H__
