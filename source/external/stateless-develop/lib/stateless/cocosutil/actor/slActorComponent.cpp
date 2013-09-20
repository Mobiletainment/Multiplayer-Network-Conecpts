//!---------------------------------------------------------------------------
//! \file "slActorComponent.cpp"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2012-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------


#include "stdafx.h"
#include "slActorComponent.h"
#include "slActorNode.h"

namespace sl	{

	ActorComponent::ActorComponent()
		:_actorNode(nullptr)
	{
	}

	ActorComponent::~ActorComponent()
	{

	}

	void ActorComponent::createActorNode(CCDictionary* parameters)	{
		if(parameters == nullptr)	{
			// the actor itself does not need any parameters right now
		}

		// TODO: think about adding a reference here
		_actorNode = ActorNode::create(parameters);

		ComponentArray* components(_actorNode->getComponents());
		components->addComponent(this);
	}

	void ActorComponent::addToActorNode(ActorNode* actor)	{
		if(actor == nullptr)	{
			SL_PROCESS_APP()->log(ELogType_Error, "%s failed - actor == nullptr", __FUNCTION__);
			return;
		}
		if(_actorNode != nullptr)	{
			SL_PROCESS_APP()->log(ELogType_Error, "%s failed - _actor != nullptr", __FUNCTION__);
			return;
		}
		_actorNode = actor;
		ComponentArray* components(actor->getComponents());
		components->addComponent(this);
	}


	bool ActorComponent::isActorNodeDestroyed() const	{
		bool isDestroyed(false);
		if(getActorNode())	{
			isDestroyed = getActorNode()->isDestroyed();
		}
		return isDestroyed;
	}


}


