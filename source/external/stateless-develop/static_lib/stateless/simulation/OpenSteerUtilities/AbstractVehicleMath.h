#ifndef __ABSTRACTVEHICLEMATH_H__
#define __ABSTRACTVEHICLEMATH_H__

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
#ifdef EDUNET_INCLUDE_OPENSTEER
#undef EDUNET_INCLUDE_OPENSTEER
#endif
#define EDUNET_INCLUDE_OPENSTEER 1
#include "EduNetCore/EduNetMacros.h"
#include "OpenSteerUtilities/OpenSteerUTTypes.h"

#include "LinearMath/btVector3.h"
#include "LinearMath/btTransform.h"
#include "LinearMath/btTransformUtil.h"

namespace OpenSteer
{
	//-------------------------------------------------------------------------
	EF_FORCEINLINE
	bool isValidFloat( float fValue )
	{
		unsigned long ulValue = *((unsigned long*)&fValue);
		return !(ulValue == 0xffc00000);
	}

	//-------------------------------------------------------------------------
	EF_FORCEINLINE
	bool isValidDouble( double dValue )
	{
		unsigned long ulLower = *((unsigned long*)&dValue);
		unsigned long ulUpper = *(((unsigned long*)&dValue) + 1);
		return !((ulLower == 0x0) && (ulUpper == 0xfff80000));
	}

	//-------------------------------------------------------------------------
	EF_FORCEINLINE
	bool isValidQuaterion( const btQuaternion& kRotation )
	{
		return ( isValidFloat( kRotation.getX() ) &&
			isValidFloat( kRotation.getY() ) &&
			isValidFloat( kRotation.getZ() ) &&
			isValidFloat( kRotation.getZ() )
			);
	}

	//-------------------------------------------------------------------------
	EF_FORCEINLINE
	bool isValidVector( const osVector3& kVector )
	{
		return ( isValidFloat( kVector.x ) &&
			isValidFloat( kVector.y ) &&
			isValidFloat( kVector.z )
			);
	}

	//-------------------------------------------------------------------------
	void localToWorldSpace( const osAbstractVehicle& vehicle, const osVector3& source, osVector3& target );

	void writeRotationMatrixToLocalSpace( const btMatrix3x3& kWorldRotation, osLocalSpaceData& localSpace );
	void writeTransformToLocalSpace( const btTransform& worldTransform, osLocalSpaceData& localSpace );
	void writeLocalSpaceToRotationMatrix( const osLocalSpaceData& localSpace, btMatrix3x3& kWorldRotation );
	bool writeLocalSpaceToTransform( const osLocalSpaceData& localSpace, btTransform& worldTransform );

	void writeTransformToLocalSpace( const btTransform& worldTransform, osAbstractLocalSpace& vehicle );
	void writeTransformToLocalSpace( const btTransform& worldTransform, osAbstractVehicle& vehicle );
	bool writeLocalSpaceToTransform( const AbstractVehicle& vehicle, btTransform& worldTransform );

	void calculateVelocity( const btTransform& worldTransform0, const btTransform& worldTransform1,
		osScalar fDeltaTime,
		osVector3& linearVelocity, osVector3& angularVelocity );

	//-------------------------------------------------------------------------
	typedef struct CompressedVector_t
	{
		char _compressedValues[3];
		unsigned short _compressionUnitFactor;
	} CompressedVector;

	//-------------------------------------------------------------------------
	class AbstractVehicleMath
	{
	public:
		static osVector3 worldDirectionToLocal( const osLocalSpaceData& localSpaceData, const osVector3& world );
		static osVector3 localDirectionToWorld( const osLocalSpaceData& localSpaceData, const osVector3& local );

		static btQuaternion computeQuaternionFromLocalSpace( const osLocalSpaceData& localSpaceData );
		static void writeQuaternionToLocalSpace( const btQuaternion& kRotation, osLocalSpaceData& localSpaceData );

		static osVector3 compressQuaternion( const btQuaternion& kRotation, char& wSign );
		static btQuaternion expandQuaternion( const osVector3& compressed, float wSign );

		static void compressUnitVector( const osVector3& source, char* target );
		static void expandUnitVector( const char* source, osVector3& target  );

		static void compressFixedLengthVector( const osVector3& source, float maxLength, CompressedVector& target );
		static void expandFixedLengthVector( const CompressedVector& source, float maxLength, osVector3& target  );

	};



}




#endif  // __ABSTRACTVEHICLEMATH_H__
