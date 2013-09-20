#include "stdafx.h"
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

#include "AbstractVehicleMath.h"
#include "slCompressed.h"

using namespace sl;
//#include "EduNetCommon/EduNetMath.h"


namespace OpenSteer
{

	// test conversion
	// 	EduNet::EmptyVehicle v1, v2;
	// 	btTransform worldTransform;
	// 	btTransform targetWorldTransform;
	// 	worldTransform.setIdentity();
	// 	writeTransformToLocalSpace( v2, worldTransform );
	// 	writeLocalSpaceToTransform( v2, targetWorldTransform );
	//-------------------------------------------------------------------------
	void writeRotationMatrixToLocalSpace( const btMatrix3x3& kWorldRotation, osLocalSpaceData& localSpace )
	{
		// calculate local coordinate system
		const btVector3& kRow0 = kWorldRotation.getRow(0);
		const btVector3& kRow1 = kWorldRotation.getRow(1);
		const btVector3& kRow2 = kWorldRotation.getRow(2);
		btVector3 kAxis[3];
		kAxis[0].setX( kRow0.getX() );
		kAxis[0].setY( kRow1.getX() );
		kAxis[0].setZ( kRow2.getX() );

		kAxis[1].setX( kRow0.getY() );
		kAxis[1].setY( kRow1.getY() );
		kAxis[1].setZ( kRow2.getY() );

		kAxis[2].setX( kRow0.getZ() );
		kAxis[2].setY( kRow1.getZ() );
		kAxis[2].setZ( kRow2.getZ() );


#if OPENSTEER_Z_ISUP
		localSpace._side = kAxis[0];
		localSpace._forward = kAxis[1];
		localSpace._up = kAxis[2];
#else
		localSpace._side = -kAxis[0];
		localSpace._forward = kAxis[2];
		localSpace._up = kAxis[1];
#endif
	}

	//-------------------------------------------------------------------------
	void writeTransformToLocalSpace( const btTransform& worldTransform, osLocalSpaceData& localSpace )
	{
		//	const btTransform& worldTransform = rigidBody->getWorldTransform();
		const btMatrix3x3& kWorldRotation = worldTransform.getBasis();
		writeRotationMatrixToLocalSpace( kWorldRotation, localSpace );
		localSpace._position = worldTransform.getOrigin();
	}

	//-------------------------------------------------------------------------
	void writeLocalSpaceToRotationMatrix( const osLocalSpaceData& localSpace, btMatrix3x3& kWorldRotation )
	{
		const btVector3 targetRow[3] =
		{
#if OPENSTEER_Z_ISUP
			localSpace._side,
			localSpace._forward,
			localSpace._up,
#else
			-localSpace._side,
			localSpace._up,
			localSpace._forward,
#endif
		};
		kWorldRotation.setValue(
			targetRow[0].x(), targetRow[1].x(), targetRow[2].x(),
			targetRow[0].y(), targetRow[1].y(), targetRow[2].y(),
			targetRow[0].z(), targetRow[1].z(), targetRow[2].z());
	}

	//-------------------------------------------------------------------------
	bool writeLocalSpaceToTransform( const osLocalSpaceData& localSpace, btTransform& worldTransform )
	{
		if( isValidVector( localSpace._forward ) == false )
		{
			return false;
		}
		if( isValidVector( localSpace._side ) == false )
		{
			return false;
		}
		if( isValidVector( localSpace._up ) == false )
		{
			return false;
		}
		if( isValidVector( localSpace._position ) == false )
		{
			return false;
		}

		worldTransform.setOrigin( localSpace._position );
		btMatrix3x3& kWorldRotation = worldTransform.getBasis();
		writeLocalSpaceToRotationMatrix( localSpace, kWorldRotation );
		return true;
	}

	//-------------------------------------------------------------------------
	void writeTransformToLocalSpace( const btTransform& worldTransform, osAbstractLocalSpace& localSpace )
	{
		writeTransformToLocalSpace( worldTransform, localSpace.accessLocalSpaceData() );
	}

	//-------------------------------------------------------------------------
	void writeTransformToLocalSpace( const btTransform& worldTransform, osAbstractVehicle& vehicle )
	{
		writeTransformToLocalSpace( worldTransform, vehicle.accessLocalSpaceData() );
	}

	//-------------------------------------------------------------------------
	bool writeLocalSpaceToTransform( const osAbstractVehicle& vehicle, btTransform& worldTransform )
	{
		return writeLocalSpaceToTransform( vehicle.getLocalSpaceData(), worldTransform );
	}

	//-------------------------------------------------------------------------
	void calculateVelocity( const btTransform& worldTransform0, const btTransform& worldTransform1,
		osScalar fDeltaTime,
		osVector3& linearVelocity, osVector3& angularVelocity )
	{
		btTransformUtil::calculateVelocity(
			worldTransform0, worldTransform1, fDeltaTime,
			linearVelocity, angularVelocity );
	}

	//-------------------------------------------------------------------------
	void localToWorldSpace( const osAbstractVehicle& vehicle, const osVector3& source, osVector3& target )
	{
		btTransform worldTransform;
		writeLocalSpaceToTransform( vehicle, worldTransform );
		target = worldTransform.getBasis() * source;
	}

	//-------------------------------------------------------------------------
	osVector3 AbstractVehicleMath::worldDirectionToLocal( const osLocalSpaceData& localSpaceData, const osVector3& world )
	{
		btTransform worldTransform;
		writeLocalSpaceToTransform( localSpaceData, worldTransform );
		osVector3 target = worldTransform.getBasis().inverse() * world;
		return target;
	}

	//-------------------------------------------------------------------------
	osVector3 AbstractVehicleMath::localDirectionToWorld( const osLocalSpaceData& localSpaceData, const osVector3& local )
	{
		btTransform worldTransform;
		writeLocalSpaceToTransform( localSpaceData, worldTransform );
		osVector3 target = worldTransform.getBasis() * local;
		return target;
	}

	//-------------------------------------------------------------------------
	btQuaternion AbstractVehicleMath::computeQuaternionFromLocalSpace( const osLocalSpaceData& localSpaceData )
	{
		btMatrix3x3 kWorldRotation;
		writeLocalSpaceToRotationMatrix( localSpaceData, kWorldRotation );
		btQuaternion kRotation;
		kWorldRotation.getRotation( kRotation );
		kRotation.normalize();
		return kRotation;
	}

	//-------------------------------------------------------------------------
	void AbstractVehicleMath::writeQuaternionToLocalSpace( const btQuaternion& kRotation, osLocalSpaceData& localSpaceData )
	{
		if( false == isValidQuaterion( kRotation ) )
		{
			return;
		}
		btMatrix3x3 kWorldRotation;
		kWorldRotation.setRotation( kRotation );
		writeRotationMatrixToLocalSpace( kWorldRotation, localSpaceData );
	}

	//-------------------------------------------------------------------------
	osVector3 AbstractVehicleMath::compressQuaternion( const btQuaternion& kRotation, char& wSign )
	{
		btQuaternion kUnitRotation = kRotation.normalized();
		osVector3 compressed( kUnitRotation.getX(), kUnitRotation.getY(), kUnitRotation.getZ() );
		wSign = ( kUnitRotation.getW() < 0 ) ? -1 : 1;
		return compressed;
	}

	//-------------------------------------------------------------------------
	btQuaternion AbstractVehicleMath::expandQuaternion( const osVector3& compressed, float wSign )
	{
		// A unit quaternion has the following property
		// w2 + x2 + y2 + z2 = 1
		// ->
		//	w2 = 1 - (x2 + y2 + z2)
		// w = sqrt( 1 - (x2 + y2 + z2) )
		btQuaternion kUnitRotation( compressed.x, compressed.y, compressed.z, 0 );
		kUnitRotation.setW(
			wSign * btSqrt( btScalar(1) -
			( kUnitRotation.getX() * kUnitRotation.getX() +
			kUnitRotation.getY() * kUnitRotation.getY() +
			kUnitRotation.getZ() * kUnitRotation.getZ()
			) ) );
		return kUnitRotation;
	}

	//-------------------------------------------------------------------------
	void AbstractVehicleMath::compressUnitVector( const osVector3& source, char* target )
	{
		const osScalar x = ::clamp( source.x, -1.0f, 1.0f );
		const osScalar y = ::clamp( source.y, -1.0f, 1.0f );
		const osScalar z = ::clamp( source.z, -1.0f, 1.0f );

		target[0] = TCompressedFixpoint<float,char,8>::writeCompress( x , -1.0f, 1.0f );
		target[1] = TCompressedFixpoint<float,char,8>::writeCompress( y , -1.0f, 1.0f );
		target[2] = TCompressedFixpoint<float,char,8>::writeCompress( z , -1.0f, 1.0f );
	}

	//-------------------------------------------------------------------------
	void AbstractVehicleMath::expandUnitVector( const char* source, osVector3& target  )
	{
		target.x = TCompressedFixpoint<float,char,8>::readInflate( source[0] , -1.0f, 1.0f );
		target.y = TCompressedFixpoint<float,char,8>::readInflate( source[1] , -1.0f, 1.0f );
		target.z = TCompressedFixpoint<float,char,8>::readInflate( source[2] , -1.0f, 1.0f );
	}

	//-------------------------------------------------------------------------
	void AbstractVehicleMath::compressFixedLengthVector( const osVector3& source, float maxLength, CompressedVector& target )
	{
		osVector3 kFixedSource = source;
		kFixedSource = kFixedSource.truncateLength( maxLength );
		const float fLength = kFixedSource.length();
		osVector3 kUnitSource = kFixedSource;
		float fUnitFactor;
		if( fLength > 0 )
		{
			kUnitSource /= fLength;
			kUnitSource.x = clamp( kUnitSource.x, -1.0f, 1.0f );
			kUnitSource.y = clamp( kUnitSource.y, -1.0f, 1.0f );
			kUnitSource.z = clamp( kUnitSource.z, -1.0f, 1.0f );
			fUnitFactor = fLength / maxLength;
			fUnitFactor = clamp( fUnitFactor, 0.0f, 1.0f );
		}
		else
		{
			fUnitFactor =
			kUnitSource.x =
			kUnitSource.y =
			kUnitSource.z = 0;
		}
		target._compressedValues[0] = TCompressedFixpoint<float,char,8>::writeCompress( kUnitSource.x , -1.0f, 1.0f );
		target._compressedValues[1] = TCompressedFixpoint<float,char,8>::writeCompress( kUnitSource.y , -1.0f, 1.0f );
		target._compressedValues[2] = TCompressedFixpoint<float,char,8>::writeCompress( kUnitSource.z , -1.0f, 1.0f );
		target._compressionUnitFactor = TCompressedFixpoint<float,short,16>::writeCompress( fUnitFactor, 0.0f, 1.0f );
	}

	//-------------------------------------------------------------------------
	void AbstractVehicleMath::expandFixedLengthVector( const CompressedVector& source, float maxLength, osVector3& target  )
	{
		// expand
		target.x = TCompressedFixpoint<float,char,8>::readInflate( source._compressedValues[0] , -1.0f, 1.0f );
		target.y = TCompressedFixpoint<float,char,8>::readInflate( source._compressedValues[1] , -1.0f, 1.0f );
		target.z = TCompressedFixpoint<float,char,8>::readInflate( source._compressedValues[2] , -1.0f, 1.0f );
		const float fLength = target.length();
		if( fLength > 0 )
		{
			target /= fLength;
			const float fUnitFactor = TCompressedFixpoint<float,short,16>::readInflate( source._compressionUnitFactor, 0.0f, 1.0f );
			target *= fUnitFactor;
			target *= maxLength;
		}
	}



} // end namespace OpenSteer

