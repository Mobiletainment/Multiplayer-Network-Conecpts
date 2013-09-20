#ifndef __ENTITY_H__
#define __ENTITY_H__

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
#include "OpenSteer/AbstractEntity.h"
#include "OpenSteer/AbstractPlayer.h"

//-----------------------------------------------------------------------------
namespace OpenSteer {

	AbstractUpdated* castToAbstractUpdated( AbstractEntity* entity );
	AbstractPlayer* castToAbstractPlayer( AbstractEntity* entity );
	AbstractPlugin* castToAbstractPlugin( AbstractEntity* entity );
	AbstractEntity* castToAbstractEntity( AbstractPlugin* plugin );

	const AbstractPlayer* castToAbstractPlayer( const AbstractEntity* entity );
	const AbstractPlugin* castToAbstractPlugin( const AbstractEntity* entity );
	const AbstractEntity* castToAbstractEntity( const AbstractPlugin* plugin );

	//-------------------------------------------------------------------------
	template <class Super>
	class EntityPossessionMixin : public Super
	{
		OS_DECLARE_BASE( Super )
	public:
		EntityPossessionMixin():
			_possessor(nullptr),
			_possessed(nullptr)
		{
		}

		virtual ~EntityPossessionMixin()
		{
			if( getControlledEntity() != nullptr)	{
				play( nullptr );
			}
			AbstractPlayer* player = getPlayer();
			if( player != nullptr )	{
				player->play( nullptr );
			}
		}

		//---------------------------------------------------------------------
		// AbstractEntity interface
		virtual void play( AbstractEntity* entity )	{
			if( isPlayer() == false )	{
				// not a player
				return;
			}
			AbstractEntity* thisEntity = dynamic_cast<AbstractEntity*>( this );
			AbstractPlayer* thisPlayer = castToAbstractPlayer( thisEntity );
			if( entity != nullptr )	{
				AbstractEntity* currentlyPossessed(_possessed);
				if(currentlyPossessed != nullptr)	{
					// release current possession
					play(nullptr);
				}

				entity->possessBy( thisEntity );
				AbstractPlayer* player = entity->getPlayer();
				if( player == thisPlayer )	{
					// play succeeded
					_possessed = entity;
				}
				else	{
					// retry playing the old entity ?
				}
			}
			else	{
				if( _possessed != nullptr )	{
					_possessed->setPossessor( nullptr );
				}
				_possessed = nullptr;
			}
		};

		virtual void setPossessor( AbstractEntity* entity )	{
			_possessor = entity;
		}

		virtual void possessBy( AbstractEntity* entity )	{
			if( _possessor != nullptr )
			{
				_possessor->play( nullptr );
			}
			setPossessor( entity );
		}

		virtual AbstractPlayer* getPlayer( void ) const	{
			return castToAbstractPlayer( _possessor );
		}

		virtual AbstractEntity* getControlledEntity( void ) const	{
			return _possessed;
		}

		virtual bool isPossessed( void ) const 	{
			return (getPlayer() != nullptr);
		}

		virtual bool isPlayer( void ) const		{
			EntityPossessionMixin* thisObject = (EntityPossessionMixin*)this;
			AbstractEntity* thisEntity = dynamic_cast<AbstractEntity*>( thisObject );
			AbstractPlayer* thisPlayer = castToAbstractPlayer( thisEntity );
			return ( thisPlayer != nullptr );
		}
	private:
		AbstractEntity* _possessor;
		AbstractEntity* _possessed;

	};

	//-------------------------------------------------------------------------
	class EntityInstance
	{
	public:
		EntityInstance()
			:_entityId( _instanceTracker.constructor() )
			,_netWorkId( NetworkId(0) )
			,_isRemoteObject( false )
		{

		}

		virtual ~EntityInstance()	{
		  _instanceTracker.destructor();
		}

		virtual EntityClassId getClassId( void ) const
		{
		  return OS_CID_UNKNOWN;
		}

		InstanceTracker::Id getEntityId( void ) const	{
			return _entityId; 
		}

		NetworkId getNetworkId( void ) const	{
			return _netWorkId;
		}

		void setNetworkId( NetworkId id )	{
			_netWorkId = id;
		}

		void setIsRemoteObject( bool isRemote )	{
			_isRemoteObject = isRemote;
		}

		bool isRemoteObject( void ) const	{
			return _isRemoteObject;
		}

		virtual const char* name (void) const	{
			return "Unknown";
		}

	private:
		InstanceTracker::Id _entityId;
		NetworkId _netWorkId;
		bool _isRemoteObject;

		static InstanceTracker _instanceTracker;
	};

//	typedef EntityPossessionMixin<EntityInstance_0> EntityInstance;

	//-------------------------------------------------------------------------
	template <class Super>
	class EntityMixin : public EntityPossessionMixin<Super>
	{
	public:
		virtual ~EntityMixin()
		{
		}

		OS_IMPLEMENT_CLASSNAME( EntityPossessionMixin )

		virtual AbstractEntity* cloneEntity( void ) const
		{
			return nullptr;
		}

		virtual EntityClassId getClassId( void ) const
		{
			return _instance.getClassId( );
		}

		virtual InstanceTracker::Id getEntityId( void ) const
		{
			return _instance.getEntityId();
		}

		virtual NetworkId getNetworkId( void ) const
		{
			return _instance.getNetworkId();
		}

		virtual void setNetworkId( NetworkId id )
		{
			_instance.setNetworkId( id );
		}

		virtual void setIsRemoteObject( bool isRemote )
		{
			_instance.setIsRemoteObject( isRemote );
		}

		virtual bool isRemoteObject( void ) const
		{
			return _instance.isRemoteObject();
		}

		virtual const char* name (void) const
		{
//			return _instance.name();
			return getClassName();
		}

	private:
		EntityInstance _instance;
	};

	//-------------------------------------------------------------------------
	template <class Super>
	class EntityParentMixin : public Super
	{
	public:
		EntityParentMixin():_parentEntity(nullptr)
		{
		}

		virtual ~EntityParentMixin()
		{
		}

		//---------------------------------------------------------------------
		// AbstractEntity interface
		virtual void setParentEntity( AbstractEntity* parentEntity )
		{
			_parentEntity = parentEntity;
		}

		//! return the parent of this entity
		virtual AbstractEntity* getParentEntity( void ) const
		{
			return _parentEntity;
		}

		virtual AbstractPlugin* getHostPlugin( void ) const
		{
			return castToAbstractPlugin( getParentEntity() );
		}

	private:
		AbstractEntity* _parentEntity;

	};

	//-------------------------------------------------------------------------
	template <class Super>
	class EntityVisibilityMixin : public Super
	{
	public:
		EntityVisibilityMixin()
			:_isVisible(true)
			,_userObject(nullptr)
		{
		}

		virtual ~EntityVisibilityMixin()
		{
		}

		//---------------------------------------------------------------------
		// AbstractEntity interface
		virtual void setVisible( bool value )
		{
			_isVisible = value;
		}

		virtual bool isVisible( void ) const
		{
			return _isVisible;
		}

		virtual void setUserobject( void* value ) OS_SEALED
		{
			_userObject = value;
		}

		virtual void* getUserobject( void ) const OS_SEALED
		{
			return _userObject;
		}



	private:
		void* _userObject;
		bool _isVisible;

	};

	//-------------------------------------------------------------------------
	template <class Super, EntityClassId classId = OS_CID_UNKNOWN>
	class EntityClassIdMixin : public Super
	{
	public:
		EntityClassIdMixin()
		{
		}

		virtual ~EntityClassIdMixin()
		{
		}

		//---------------------------------------------------------------------
		// AbstractEntity interface
		virtual AbstractEntity* cloneEntity( void ) const
		{
			return OS_NEW EntityClassIdMixin();
		}

		virtual EntityClassId getClassId( void ) const
		{
			static EntityClassId sClassId = classId;
			return sClassId;
		}


	private:

	};

	typedef EntityVisibilityMixin<AbstractEntity> TVisibleEntity;
	typedef EntityParentMixin<TVisibleEntity> TEntityParent;
	typedef EntityMixin<TEntityParent> TEntity;
	typedef TEntity Entity;


}

#endif //! __ENTITY_H__
