//!---------------------------------------------------------------------------
//! \file "nlProtocolStructures.h"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2012-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------


#pragma once
#ifndef __SLPROTOCOLSTRUCTURES_H__
#define __SLPROTOCOLSTRUCTURES_H__

#include <stateless/cocosgame/slCocosGame.h>

namespace nl	{

	// TODO @student : add missing members to datagram structures

	typedef struct TControllerValues	{
		float _leftRight;
		float _forwardBackward;
		float _shoot;
		unsigned int _killCount;
		RakNet::NetworkID _controlledReplicaNetworkId;
		SLSize _updateTick;
	} ControllerValues;

	typedef struct TCompressed_ControllerValues	{
		char _leftRight;
		char _forwardBackward;
		char _shoot;
		unsigned int _killCount;
		SLSize _updateTick;
		RakNet::NetworkID _controlledReplicaNetworkId;
	} Compressed_ControllerValues;

	typedef struct TDynamic2DActorDatagram	{
		float _x;
		float _y;
		float _fx;
		float _fy;
		float _lvx;
		float _lvy;
		float _avz;
		SLSize _updateTick;
	} Dynamic2DActorDatagram;

	// TODO @David: compressed structures implemented but not used yet
	typedef struct TCompressedDynamic2DActorDatagram	{
		short _x;
		short _y;
		float _lvx;
		float _lvy;
		SLSize _updateTick;
		float _fx;
		float _fy;
		float _avz;
	} Compressed_Dynamic2DActorDatagram;


	class ProtocolUtilities	{
	public:
		//! @brief blends from client to server with serverVecFactor
		//! @return the linear interpolation between clientVec and serverVec vector 
		//!  * @param clientVec The source vector 
		//!  * @param serverVec The target vector 
		//!  * @param serverVecFactor The ration of clientVec to serverVec (serverVecFactor = 0 => return clientVec, serverVecFactor=1 => return serverVec)
		static Vec3 interpolateNetVector(const Vec3& clientVec, const Vec3& serverVec, float serverVecFactor);

	};
}


#endif // __SLPROTOCOLSTRUCTURES_H__
