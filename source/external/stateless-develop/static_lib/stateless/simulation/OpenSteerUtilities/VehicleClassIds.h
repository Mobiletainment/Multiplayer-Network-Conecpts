#ifndef __VEHICLECLASSIDS_H__
#define __VEHICLECLASSIDS_H__

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

#include "EduNetCommon/EduNetCommon.h"
#include "OpenSteer/Entity.h"

//-----------------------------------------------------------------------------
namespace OpenSteer
{

	//-------------------------------------------------------------------------
	// implement entity class id
	static const EntityClassId g_clasId_NetPedestrian( 1 + OS_CID_LASTRESERVED );	
	static const EntityClassId g_clasId_CtfBaseVehicle( 2 + OS_CID_LASTRESERVED );
	static const EntityClassId g_clasId_CtfSeekerVehicle( 3 + OS_CID_LASTRESERVED );
	static const EntityClassId g_clasId_CtfEnemyVehicle( 4 + OS_CID_LASTRESERVED );
	static const EntityClassId g_clasId_NetBoid( 5 + OS_CID_LASTRESERVED );
	static const EntityClassId g_clasId_NetSoccerPlayer( 6 + OS_CID_LASTRESERVED );
	static const EntityClassId g_clasId_NetSoccerBall( 7 + OS_CID_LASTRESERVED );
	static const EntityClassId g_clasId_StudentVehicle( 8 + OS_CID_LASTRESERVED );
}

//-----------------------------------------------------------------------------
// define entity class id
#define ET_CID_UNKNOWN OS_CID_UNKNOWN
#define ET_CID_NETPEDESTRIAN OpenSteer::g_clasId_NetPedestrian
#define ET_CID_CTF_BASE_VEHICLE OpenSteer::g_clasId_CtfBaseVehicle
#define ET_CID_CTF_ENEMY_VEHICLE OpenSteer::g_clasId_CtfSeekerVehicle
#define ET_CID_CTF_SEEKER_VEHICLE OpenSteer::g_clasId_CtfEnemyVehicle
#define ET_CID_NETBOID OpenSteer::g_clasId_NetBoid
#define ET_CID_NETSOCCER_PLAYER OpenSteer::g_clasId_NetSoccerPlayer
#define ET_CID_NETSOCCER_BALL OpenSteer::g_clasId_NetSoccerBall
#define ET_CID_STUDENT_VEHICLE OpenSteer::g_clasId_StudentVehicle


#endif // __VEHICLECLASSIDS_H__