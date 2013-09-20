//!---------------------------------------------------------------------------
//! \file "slActorComponent.h"
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
#ifndef __SLACTORCOMPONENT_H__
#define __SLACTORCOMPONENT_H__

#include "../components/slComponent.h"

namespace sl	{

	class ActorNode;

	class slCocosUtil_API ActorComponent : public Component	{

		SL_DECLARE_BASE(Component)
	public:
		CREATE_FUNC(ActorComponent)

		//! creates an actor node
		virtual void createActorNode(CCDictionary* parameters = nullptr) SL_VIRTUAL_DECLARE;

		void addToActorNode(ActorNode* actor);

		bool isActorNodeDestroyed() const;

		CC_SYNTHESIZE(ActorNode*, _actorNode, ActorNode)

	protected:
		ActorComponent();
		virtual ~ActorComponent();

	};

	typedef sltl::TCCSmartPointer<ActorComponent> ActorComponentPtr;

}


#endif // __SLACTORCOMPONENT_H__
