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
#include "stdafx.h"

#include "OpenSteer/GlobalData.h"
#include "OpenSteer/AbstractPlugin.h"
#include "OpenSteer/AbstractVehicle.h"
#include "OpenSteer/Player.h"
#include "OpenSteer/PluginRegistry.h"
#include "OpenSteer/Camera.h"
#include "OpenSteer/Clock.h"
#include "OpenSteer/GlobalSelection.h"
#include "OpenSteer/AbstractRenderer.h"


//-----------------------------------------------------------------------------
namespace OpenSteer {

	static GlobalData* g_globalData = nullptr;
	GlobalData* GlobalData::ms_globalDataInstance = nullptr;

	class GlobalDataConstructor
	{
	public:
		GlobalDataConstructor()
		{
			g_globalData = new GlobalData();
		};
		virtual ~GlobalDataConstructor()
		{
			delete g_globalData;
		};

	};

	//-------------------------------------------------------------------------
	void GlobalData::_SDMInitApp( EduNet::IProfile* profile )
	{
		if( GlobalData::hasInstance() == false )
		{
			GlobalSelection::_SDMInitApp( );

			static GlobalDataConstructor kConstructor;
			GlobalData* globalData = g_globalData;
			globalData->initializeGlobalData();
			globalData->_profile = profile;
			GlobalData::setInstance( globalData );
		}
	}

	//-------------------------------------------------------------------------
	void GlobalData::_SDMInitDLL( GlobalData* globalData )
	{
		globalData->_isDll = true;
		GlobalData::setInstance( globalData );
	}

	//-------------------------------------------------------------------------
	void GlobalData::setInstance( GlobalData* globalData )
	{
		GlobalData::ms_globalDataInstance = globalData;
	}

	//-------------------------------------------------------------------------
	GlobalData* GlobalData::getInstance( void )
	{
		handleGlobalDataInstanceFailure();
		assert( GlobalData::ms_globalDataInstance != nullptr );
		return GlobalData::ms_globalDataInstance;
	}

	//-------------------------------------------------------------------------
	bool GlobalData::hasInstance( void)
	{
		return ( GlobalData::ms_globalDataInstance != nullptr );
	}

	//-------------------------------------------------------------------------
	AbstractPlayer* GlobalData::accessSimpleLocalPlayer( void )
	{
		return GlobalData::getInstance()->_simpleLocalPlayer;
	}

	//-------------------------------------------------------------------------
	EduNet::IProfile* GlobalData::accessProfile( void )
	{
		return GlobalData::getInstance()->_profile;
	}

	//-------------------------------------------------------------------------
	Camera* GlobalData::accessCamera( void )
	{
		// pointer to debug validity
		GlobalData* globalData = GlobalData::getInstance();

		// one static camera instance that automatically tracks selected vehicle
		return GlobalData::getInstance()->_camera;
	}

	//-------------------------------------------------------------------------
	Clock* GlobalData::accessClock( void )
	{
		// pointer to debug validity
		GlobalData* globalData = GlobalData::getInstance();
		return GlobalData::getInstance()->_clock;
	}

	//-------------------------------------------------------------------------
	PluginRegistry* GlobalData::accessPluginRegistry( void )
	{
		return GlobalData::getInstance()->_pluginRegistry;
	}

	//-------------------------------------------------------------------------
	AbstractRenderer* GlobalData::accessRenderer( void )
	{
		return GlobalData::getInstance()->_renderer;
	}
	
	//-------------------------------------------------------------------------
	void GlobalData::setRenderer( AbstractRenderer* renderer )
	{
		GlobalData::getInstance()->_renderer = renderer;
	}

	//-------------------------------------------------------------------------
	bool GlobalData::getEnableAnnotation( void )
	{
		return GlobalData::getInstance()->_enableAnnotation;
	}

	//-------------------------------------------------------------------------
	void GlobalData::setEnableAnnotation( bool value )
	{
		GlobalData::getInstance()->_enableAnnotation = value;
	}

	//-------------------------------------------------------------------------
	bool GlobalData::getDrawPhaseActive( void )
	{
		return GlobalData::getInstance()->_drawPhaseActive;
	}

	//-------------------------------------------------------------------------
	void GlobalData::setDrawPhaseActive( bool value )
	{
		GlobalData::getInstance()->_drawPhaseActive = value;
	}


	//-------------------------------------------------------------------------
	GlobalData::GlobalData( void ):
		_simpleLocalPlayer( nullptr ),
		_simpleController( nullptr ),
		_pluginRegistry( nullptr ),
		_camera( nullptr ),
		_clock( nullptr ),
		_renderer( nullptr ),
		_showClientNetworkTrail(0),
		_showServerNetworkTrail(0),
		_netWriteFPS(20),
		_collect3DAnnotations(0),
		_debugNetStats(1),
		_showMotionStatePlot(0),
		_enableAnnotation(true),
		_drawPhaseActive(false),
		_steeringForceFPS(30),
		_isDll(false),
		_netInterpolationDistanceThreshHold(0.05f),
		_netPositionInterpolationFactor(0.5f)
	{
		memset( _replicationDataConfig, 0, sizeof(int) * ESerializeDataType_Count);
		_replicationDataConfig[ESerializeDataType_Position] = 1;
		_replicationDataConfig[ESerializeDataType_Forward] = 1;
		_replicationDataConfig[ESerializeDataType_UpdateTicks] = 1;
	
		_replicationDataBytes[ESerializeDataType_Position] = sizeof(Vec3) + 1;
		_replicationDataBytes[ESerializeDataType_Forward] = sizeof(Vec3) + 1;
		_replicationDataBytes[ESerializeDataType_Side] = sizeof(Vec3) + 1;
		_replicationDataBytes[ESerializeDataType_Up] = sizeof(Vec3) + 1;
		_replicationDataBytes[ESerializeDataType_Force] = sizeof(Vec3) + 1;
		_replicationDataBytes[ESerializeDataType_Radius] = sizeof(osScalar) + 1;
		_replicationDataBytes[ESerializeDataType_Speed] = sizeof(osScalar) + 1;
		_replicationDataBytes[ESerializeDataType_Orientation] = 0;
//		_replicationDataBytes[ESerializeDataType_Orientation] = sizeof(btQuaternion) + 1;
		_replicationDataBytes[ESerializeDataType_CompressedOrientation1] = sizeof(Vec3) + 2;
		_replicationDataBytes[ESerializeDataType_CompressedOrientation2] = sizeof(char) * 3 + 2;
		_replicationDataBytes[ESerializeDataType_CompressedForce] = 0;
//		_replicationDataBytes[ESerializeDataType_CompressedForce] = sizeof(CompressedVector) + 1;
		_replicationDataBytes[ESerializeDataType_AngularVelocity] = sizeof(Vec3) + 1;
		_replicationDataBytes[ESerializeDataType_LinearVelocity] = sizeof(Vec3) + 1;
		_replicationDataBytes[ESerializeDataType_UpdateTicks] = sizeof(size_t) + 1;
		_replicationDataBytes[ESerializeDataType_ControllerAction] = sizeof(float) + 1;
		
		
	}

	//-------------------------------------------------------------------------
	GlobalData::~GlobalData()
	{
		if( this == GlobalData::ms_globalDataInstance )
		{
			GlobalData::ms_globalDataInstance = nullptr;
		}
	}

	void GlobalData::initializeGlobalData( void )
	{
		static PluginRegistry kPluginRegistry;
		_pluginRegistry = &kPluginRegistry; 

		static Camera kCamera;
		_camera = &kCamera;

		static Clock kClock;
		_clock = &kClock;


		static Player player(true);
		_simpleLocalPlayer = &player;
		if( player.getController() == nullptr )
		{
			static Controller controller;
			_simpleController = &controller;
			player.setController( &controller );
		}
	}

} //! namespace OpenSteer    


