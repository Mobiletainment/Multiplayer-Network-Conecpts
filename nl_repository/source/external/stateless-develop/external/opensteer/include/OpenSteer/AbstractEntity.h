#ifndef __ABSTRACTENTITY_H__
#define __ABSTRACTENTITY_H__

//-----------------------------------------------------------------------------
//! Copyright (c) 2011, Jan Fietz, Cyrus Preuss
//! All rights reserved.
//! 
//! Redistribution and use in source and binary forms, with or without modification, 
//! are permitted provided that the following conditions are met:
//! 
//! * Redistributions of source code must retain the above copyright notice, 
//!   this list of conditions and the following disclaimer.
//! * Redistributions in binary form must reproduce the above copyright notice, 
//!   this list of conditions and the following disclaimer in the documentation 
//!   and/or other materials provided with the distribution.
//! * Neither the name of EduNetGames nor the names of its contributors
//!   may be used to endorse or promote products derived from this software
//!   without specific prior written permission.
//! 
//! THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//! ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
//! WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
//! IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
//! INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
//! (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
//! LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON 
//! ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
//! (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
//! EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#include "OpenSteer/OpenSteerTypes.h"
#include "OpenSteer/OpenSteerMacros.h"
#include "OpenSteer/InstanceTracker.h"

enum OS_CID
{
	OS_CID_UNKNOWN,
	OS_CID_ENTITY,
	OS_CID_LOCALSPACE,
	OS_CID_PLAYER,
	OS_CID_CLIENT_PLAYER,
	OS_CID_PLAYERCONTROLLER,
	OS_CID_OBSTACLE,
	OS_CID_SPHEREOBSTACLE,
	OS_CID_BOXOBSTACLE,
	OS_CID_PLANEOBSTACLE,
	OS_CID_RECTANGLEOBSTACLE,

	OS_CID__LASTRESERVED = 1000,
};

//-----------------------------------------------------------------------------
namespace OpenSteer {

	typedef uint64_t NetworkId; 
	typedef OS_CID EntityClassId; 

	class AbstractPlayer;
	class AbstractPlugin;

	//-------------------------------------------------------------------------
	class AbstractEntity
	{
	public:
		virtual ~AbstractEntity() { /* Nothing to do. */ }

		OS_DECLARE_CLASSNAME

		//! return a pointer to a cloned instance of this entity
		virtual AbstractEntity* cloneEntity( void ) const OS_ABSTRACT;

		//! return the unique class id of this object
		virtual EntityClassId getClassId( void ) const OS_ABSTRACT;

		//! return the unique instance id of this object
		virtual InstanceTracker::Id getEntityId( void ) const OS_ABSTRACT;

		//! return the unique network id of this object
		virtual NetworkId getNetworkId( void ) const OS_ABSTRACT;

		//! set the unique network id of this object
		virtual void setNetworkId( NetworkId ) OS_ABSTRACT;

		//! set if this object is a remote object or not
		virtual void setIsRemoteObject( bool isRemote ) OS_ABSTRACT;

		//! return if this object is a remote object
		virtual bool isRemoteObject( void ) const OS_ABSTRACT;

		//! return a pointer to this instance's character string name
		virtual const char* name (void) const OS_ABSTRACT;

		//! assign a parent to this entity
		virtual void setParentEntity( AbstractEntity* ) OS_ABSTRACT;

		//! return the parent of this entity
		virtual AbstractEntity* getParentEntity( void ) const OS_ABSTRACT; 

		//! return the host plugin of this entity
		virtual AbstractPlugin* getHostPlugin( void ) const OS_ABSTRACT; 

		//! call to let a player play the specified entity
		virtual void play( AbstractEntity* ) OS_ABSTRACT;

		//! called in case a player plays this entity
		virtual void possessBy( AbstractEntity* ) OS_ABSTRACT;

		//! do not call by yourself !!!
		virtual void setPossessor( AbstractEntity* ) OS_ABSTRACT;

		//! return the player of this entity
		virtual AbstractPlayer* getPlayer( void ) const OS_ABSTRACT;

		//! return true if this entity is possessed by a player
		virtual bool isPossessed( void ) const OS_ABSTRACT;

		//! return true if this entity is a player
		virtual bool isPlayer( void ) const  OS_ABSTRACT;

		//! return true if this entity is visible
		virtual bool isVisible( void ) const  OS_ABSTRACT;

		//! set if this entity is visible
		virtual void setVisible( bool value )  OS_ABSTRACT;

		//! set a user object to this entity
		virtual void setUserobject( void* value ) OS_ABSTRACT;

		//! return the user object of this entity
		virtual void* getUserobject( void ) const OS_ABSTRACT; 
	};


}

#endif //! __ABSTRACTENTITY_H__
