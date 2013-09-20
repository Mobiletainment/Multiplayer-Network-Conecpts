#ifndef __ABSTRACTVEHICLEUPDATE_H__
#define __ABSTRACTVEHICLEUPDATE_H__

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

#include "EduNetCore/EduNetMacros.h"
#include "slUpdatePeriod.h"
#include "OpenSteer/Annotation.h"
#include "OpenSteerUtilities/PhysicsMotionState.h"

class btTransform;

namespace OpenSteer
{

	typedef AnnotationMixin<AbstractUpdated> TAbstractAnnotatedUpdated;
	//-------------------------------------------------------------------------
	class AbstractVehicleUpdate : public TAbstractAnnotatedUpdated {
	public:
		AbstractVehicleUpdate( AbstractVehicle* vehicle ):
		  _vehicle( vehicle ), _customUpdated(NULL), _enabled( true )
		  {
		  }
		  virtual ~AbstractVehicleUpdate(){}

		  //-------------------------------------------------------------------
		  // interface AbstractUpdated
		  virtual void update( const osScalar currentTime, const osScalar elapsedTime )
		  {
			  AbstractUpdated* customUpdated(getCustomUpdated());
			  if( customUpdated != nullptr )
			  {
				  // in case the custom updater decides to call the base class
				  // prevent infinite recursion, store the custom updater locally
				  // and restore it once done with the update
				  setCustomUpdated( nullptr );
				  customUpdated->updateCustom( this, currentTime, elapsedTime );
				  setCustomUpdated( customUpdated );
			  }
		  }

		  virtual void updateCustom( AbstractUpdated* /*parent*/, const osScalar /*currentTime*/, const osScalar /*elapsedTime*/ )
		  {
			  // nothing to do here
			  return;
		  }

		  virtual void setCustomUpdated( AbstractUpdated* updated )
		  {
			  _customUpdated = updated;
		  }

		  virtual AbstractUpdated* getCustomUpdated( void ) const
		  {
			  return _customUpdated;
		  }

		  //-------------------------------------------------------------------
		  virtual void setVehicle( AbstractVehicle* vehicle )
		  {
			  _vehicle = vehicle;
		  }

		  bool isVehicleUpdate( void ) const
		  {
			  return (_vehicle != NULL);
		  }

		  AbstractVehicle& vehicle( void ) const
		  {
			  assert( _vehicle != NULL );
			  return *_vehicle;
		  }

		  virtual bool isEnabled( void ) const { return _enabled; };
		  virtual void setEnabled( bool bEnabled ){ _enabled = bEnabled; };

	protected:
		AbstractVehicle* _vehicle;
		AbstractUpdated* _customUpdated;
		bool _enabled;

	};

	//----------------------------------------------------------------------------
	class AbstractServerVehicleUpdate : public AbstractVehicleUpdate
	{
	public:
	};

	//----------------------------------------------------------------------------
	class AbstractClientVehicleUpdate : public AbstractVehicleUpdate
	{
	public:
	};


}




#endif  // __ABSTRACTVEHICLEUPDATE_H__
