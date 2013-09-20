//!---------------------------------------------------------------------------
//! \file "slAbstractActor.cpp"
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
#include "slAbstractActor.h"
#include "slActorNode.h"



namespace sl	{


	//-------------------------------------------------------------------------
	// AbstractActor
	//-------------------------------------------------------------------------

	AbstractActor::AbstractActor()
		:_constructionDictionary(nullptr)
		,_instigator(nullptr)
	{
		// initially we like debugging so we activate some flags
		getActorFlags().addFlag(EActorFlag_DrawVehicle);
	}

	AbstractActor::~AbstractActor()
	{
	}

	ActorNode* AbstractActor::getActorNode() const
	{
		return dynamic_cast<ActorNode*>(getParent());
	}

	bool AbstractActor::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
	{
		if (!isTouchInside(pTouch) )
		{
			return false;
		}
		return true;
	}

	void AbstractActor::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
	{
		if (isTouchInside(pTouch) )	{
			setTouchEnabled(false);
			// note: getParent as the sprite is contained inside a master node
			getParent()->removeFromParent();
		}
	}

	CCPoint AbstractActor::getTouchLocation(CCTouch* touch)
	{
		// Get the touch position
		CCPoint touchLocation = touch->getLocation();            
		// Convert to the node space of this class
		touchLocation = convertToNodeSpace(touchLocation); 
		return touchLocation;
	}

	bool AbstractActor::isTouchInside(CCTouch* touch)
	{
		// Get the touch position
		CCNode* parent(getParent());
		if(parent != nullptr)	{
			CCPoint touchLocation = touch->getLocation();
			touchLocation = parent->convertToNodeSpace(touchLocation);
			CCRect bBox=boundingBox();
			return bBox.containsPoint(touchLocation);
		}
		return false;
	}

	bool AbstractActor::init()
	{
		SLBaseClass::init();
		setTouchEnabled(true);
		return true;
	}

	void AbstractActor::setTouchEnabled( bool value )
	{
		CCTouchDispatcher* dispatcher(CCDirector::sharedDirector()->getTouchDispatcher());
		if(value)	{
			dispatcher->addTargetedDelegate(this, 0, true);
		}
		else	{
			dispatcher->removeDelegate(this);
		}

	}

	void AbstractActor::onEnter()
	{
		SLBaseClass::onEnter();
	}

	void AbstractActor::onExit()
	{
		setInstigator(nullptr);
		setConstructionDictionary(nullptr);
		SLBaseClass::onExit();
	}





}


