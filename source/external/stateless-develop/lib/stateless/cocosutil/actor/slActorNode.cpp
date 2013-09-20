//!---------------------------------------------------------------------------
//! \file "slActorNode.cpp"
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
#include "slActorNode.h"

namespace sl	{

	ActorNode::ActorNode()
	{
		_components = ComponentArray::create();
		addChild(_components);
	}

	ActorNode::~ActorNode()
	{

	}

	ActorNode* ActorNode::create( CCDictionary* dictionary )
	{
		ActorNode *actor = new ActorNode();
		if (actor && actor->initWithDictionary(dictionary))
		{
			actor->autorelease();
			return actor;
		}
		CC_SAFE_DELETE(actor);
		return NULL;
	}

	bool ActorNode::initWithDictionary( CCDictionary* dictionary )
	{
		return true;
	}


	void ActorNode::preUpdateComponents( float dt )
	{
		_components->preUpdate(dt);
	}

	void ActorNode::postUpdateComponents( float dt )
	{
		_components->postUpdate(dt);
	}

	bool ActorNode::isDestroyed() const
	{
		bool isDestroyed(const_cast<ActorNode*>(this)->getActorNodeFlags().hasFlag(EActorNodeFlag_IsDestroyed));
		return isDestroyed;
	}

}


