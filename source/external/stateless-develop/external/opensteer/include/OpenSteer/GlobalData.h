#ifndef OPENSTEER_GLOBALDATA_H
#define OPENSTEER_GLOBALDATA_H

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
#include "OpenSteer/AbstractVehicle.h"
namespace EduNet
{
	class IProfile;
}
//-----------------------------------------------------------------------------
namespace OpenSteer {
	class AbstractPlayer;
	class AbstractController;
	class PluginRegistry;
	class Camera;
	class Clock;
	class AbstractRenderer;

	class GlobalData
	{
	public:
		GlobalData();
		virtual ~GlobalData();

		static void _SDMInitApp( EduNet::IProfile* profile );
		static void _SDMInitDLL( GlobalData* globalData );
		static GlobalData* getInstance( void );
		static bool hasInstance( void);

		static AbstractPlayer* accessSimpleLocalPlayer( void );
		static Camera* accessCamera( void );
		static Clock* accessClock( void );
		
		static EduNet::IProfile* accessProfile( void );

		static PluginRegistry* accessPluginRegistry( void );
		static AbstractRenderer* accessRenderer( void );
		static void setRenderer( AbstractRenderer* renderer );

		static bool getEnableAnnotation( void );
		static void setEnableAnnotation( bool value );

		static bool getDrawPhaseActive( void );
		static void setDrawPhaseActive( bool value );


		int _replicationDataConfig[ESerializeDataType_Count];
		size_t _replicationDataBytes[ESerializeDataType_Count];
		int _showClientNetworkTrail;
		int _showServerNetworkTrail;
		osScalar _netWriteFPS;
		int _collect3DAnnotations;
		int _debugNetStats;
		int _showMotionStatePlot;
		osScalar _steeringForceFPS;
		bool _enableAnnotation;
		bool _drawPhaseActive;
		float _netInterpolationDistanceThreshHold;
		float _netPositionInterpolationFactor;

	private:
		void initializeGlobalData( void );
		
		// note: global member variable pointers
		AbstractPlayer* _simpleLocalPlayer;
		AbstractController* _simpleController;
		PluginRegistry* _pluginRegistry;
		Camera* _camera;
		Clock* _clock;
		AbstractRenderer* _renderer;
		EduNet::IProfile* _profile;
		bool _isDll;


		static void setInstance( GlobalData* globalData );
		
		static GlobalData* ms_globalDataInstance;
	};

	extern void handleGlobalDataInstanceFailure( void );

} //! namespace OpenSteer    


//-----------------------------------------------------------------------------
#endif //! OPENSTEER_GLOBALDATA_H
