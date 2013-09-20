//!---------------------------------------------------------------------------
//! \file "slCompressed.h"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2012-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------


#ifndef __SLCOMPRESSED_H__
#define __SLCOMPRESSED_H__

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

// compressing a value with a certain range into a data type with less bytes
// costs - accuracy !
//
// Example:
//		char out = TCompressedFixpoint<float,char,8>::writeCompress( 0.9f , -1.0f, 1.0f );
//
// Writes the float value 0.0f which is always between -1.0f and 1.0f into a byte. 
// The transmission needs now 24 bits less the sending the whole float.
//
//     float out = TCompressedFixpoint<float,char,8>::readInflate( bOut , -1.0f, 1.0f );
//
// Converts the received char back to the origin float.

namespace sl	{
	template<typename InType, typename OutType, int bitSize>
	class TCompressedFixpoint
	{
	private:
		enum
		{
			MaxAccessValue = 1 << bitSize,		
			BitMask   = MaxAccessValue - 1,
			Converter  = BitMask - 1,
		};

	public:

		static OutType writeCompress( InType value, const InType lowBound, const InType highBound )
		{
			assert( (value >= lowBound && value <= highBound) );
			value -= lowBound;
			value *= 1 / (highBound - lowBound);
			return static_cast<OutType>( value * Converter ) & BitMask;
		}

		static InType readInflate( OutType value, const InType lowBound, const InType highBound )
		{
			assert( (value < MaxAccessValue) );	
			InType outValue = static_cast<InType>(value & BitMask);
			outValue /= static_cast<InType>( Converter );
			outValue *= ( highBound - lowBound );
			return (lowBound + outValue);
		}
	};

}


#endif // __SLCOMPRESSED_H__
