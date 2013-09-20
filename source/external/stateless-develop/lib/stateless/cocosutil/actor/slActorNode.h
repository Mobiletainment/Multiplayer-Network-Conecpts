//!---------------------------------------------------------------------------
//! \file "slActorNode.h"
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
#ifndef __SLACTORNODE_H__
#define __SLACTORNODE_H__

#include "../slAbstractNode.h"
#include "../components/slComponent.h"

namespace sl	{

	enum EActorNodeFlags
	{
		EActorNodeFlag_Undefined = 0,
		EActorNodeFlag_IsDestroyed = 1 << 0,
		EActorNodeFlag_000 = 1 << 5,
		EActorNodeFlag_001 = 1 << 6,
		EActorNodeFlag_002 = 1 << 7,
		EActorNodeFlag_003 = 1 << 8,
		EActorNodeFlag_004 = 1 << 9,
		EActorNodeFlag_005 = 1 << 10,
		EActorNodeFlag_006 = 1 << 11,
		EActorNodeFlag_007 = 1 << 12,
		EActorNodeFlag_008 = 1 << 13,
		EActorNodeFlag_009 = 1 << 14,
		EActorNodeFlag_010 = 1 << 15,
	};

#pragma warning(push)
#pragma warning(disable:4251) // needs to have dll-interface to be used by clients of class
	class slCocosUtil_API ActorNodeFlags0	{

	};

	class slCocosUtil_API ActorNodeFlags : public sltl::FlagSetMixin<ActorNodeFlags0, EActorNodeFlags>	{

	};

#pragma warning(pop)


	//! @brief a ActorNode is an object hosting at least one visual Actor and a Component array
	//!
	//! The ActorNode allows access to the most relevant game objects
	class slCocosUtil_API ActorNode : public AbstractNode
	{
		SL_DECLARE_BASE(AbstractNode)
	public:
		static ActorNode* create(CCDictionary* dictionary);

		//! initialize with a dictionary
		virtual bool initWithDictionary(CCDictionary* dictionary) SL_VIRTUAL_DECLARE;

		//! calls perUpdate on all components
		void preUpdateComponents( float dt );

		//! calls postUpdate on all components
		void postUpdateComponents( float dt );

		//! return true in case destroyed
		virtual bool isDestroyed() const SL_VIRTUAL_DECLARE;

		//! the actor components array
		//! 
		//! do not directly add components to this array
		//! use the ActorComponents member function addToActorNode instead
		CC_SYNTHESIZE_READONLY(ComponentArray*, _components, Components);
		SL_SYNTHESIZE_GET_BY_REF(ActorNodeFlags, _actorNodeFlags, ActorNodeFlags)

	protected:
		ActorNode();
		virtual ~ActorNode();

	private:

	};

}


#endif // __SLACTORNODE_H__
