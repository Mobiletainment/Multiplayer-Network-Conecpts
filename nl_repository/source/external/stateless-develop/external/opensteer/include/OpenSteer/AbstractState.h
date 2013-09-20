#pragma once
#ifndef __ABSTRACTSTATE_H__
#define __ABSTRACTSTATE_H__

/*
 *  AbstractState.h
 *  OpenSteer
 *
 *  Created by Cyrus Preuss on 11/9/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include <vector>
#include "OpenSteer/AbstractUpdated.h"

//-----------------------------------------------------------------------------
namespace OpenSteer {
		
	//-------------------------------------------------------------------------
	class AbstractState : public AbstractUpdated  {
	public:
		virtual ~AbstractState() { /* Nothing to do. */ }
	};
	
	
	//-------------------------------------------------------------------------
	template <class entity_type, 
	typename TypeStateId = int,
	typename TypeAccumTime = osScalar,
	typename TypeDeltaTime = osScalar
	>
	class TState //: public AbstractState
	{
	public:
		
		TState( TypeStateId stateId = static_cast<TypeStateId> (-1) ):
		_stateId(stateId) 
		{}
		
		virtual ~TState() 
		{}
		
		TypeStateId getStateId( void ) const { return _stateId; }
		void setStateId( TypeStateId id ) { _stateId = id; }
		
		//this will execute when the state is entered
		virtual void enter( entity_type* ) OS_ABSTRACT;
		
		//this is the states normal update function
		virtual void execute( entity_type*, TypeAccumTime fAccumTime, TypeDeltaTime fDeltaTime ) OS_ABSTRACT;
		
		//this will execute when the state is exited
		virtual void exit( entity_type* ) OS_ABSTRACT;
		
		OS_IMPLEMENT_CLASSNAME( TState );

	protected:
//		typedef std::vector<TState< entity_type, TypeStateId, TypeAccumTime, TypeDeltaTime >::TState> TStateArray;

//		typedef TState TSingleState;
		
//		typedef std::vector<TSingleState> 
//		TStateArray;
//		TStateArray _States;
		
	private:
		TypeStateId _stateId;
		
	};

	//-------------------------------------------------------------------------
	
	//-------------------------------------------------------------------------
	template <class entity_type, 
	typename TypeStateId = int,
	typename TypeAccumTime = osScalar,
	typename TypeDeltaTime = osScalar>
	class TStateMachine : public TState< entity_type, TypeStateId, TypeAccumTime, TypeDeltaTime >
	{
	public:
		
		TStateMachine( TypeStateId stateId = static_cast<TypeStateId> (-1) ):
		TState< entity_type, TypeStateId, TypeAccumTime, TypeDeltaTime >(stateId) {}
		virtual ~TStateMachine() {}
		
		
		//this will execute when the state is entered
		virtual void enter( entity_type* ) {};//OS_ABSTRACT;
		
		//this is the states normal update function
		virtual void execute( entity_type* entity, TypeAccumTime fAccumTime, TypeDeltaTime fDeltaTime )
		{
			const size_t numStates = _States.size();
			if( numStates > 0 )
			{
				TState< entity_type, TypeStateId, TypeAccumTime, TypeDeltaTime >& kState = 
				_States.back();
				kState.execute( entity, fAccumTime, fDeltaTime );
			}
		}
		
		//this will execute when the state is exited
		virtual void exit( entity_type* ) {};//OS_ABSTRACT;
		
		void pushState( const TState< entity_type, TypeStateId, TypeAccumTime, TypeDeltaTime >& kState )
		{
			_States.push_back( kState );
		}

		void popState( void )
		{
			const size_t numStates = _States.size();
			if( numStates > 0 )
			{
				TState< entity_type, TypeStateId, TypeAccumTime, TypeDeltaTime >& kState = 
				_States.back();
				_States.pop_back();
			}
		}
		
		OS_IMPLEMENT_CLASSNAME( TStateMachine );
	private:
		TStateVector< entity_type, TypeStateId, TypeAccumTime, TypeDeltaTime > _States;
		
	};
	

	//! idee: leave definiert target state - implementiert statechange
	//!       leaveCondition() boolean
	
	
	
	
	
}



#endif //!  __ABSTRACTSTATE_H__
