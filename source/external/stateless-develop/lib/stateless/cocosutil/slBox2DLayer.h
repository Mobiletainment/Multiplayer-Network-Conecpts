//!---------------------------------------------------------------------------
//! \file "slBox2DLayer.h"
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
#ifndef __SLBOX2DLAYER_H_
#define __SLBOX2DLAYER_H_

#include "physics_nodes/CCPhysicsSprite.h"
#include "Box2D/Box2D.h"

USING_NS_CC;
USING_NS_CC_EXT;

namespace sl	{

	class slCocosUtil_API Box2DTestLayer : public CCLayer
	{
		SL_DECLARE_BASE(cocos2d::CCLayer)

			CCTexture2D* m_pSpriteTexture;    // weak ref
		b2World* world;
		//    GLESDebugDraw* m_debugDraw;

	public:
		Box2DTestLayer();
		~Box2DTestLayer();

		virtual bool init();

		void initPhysics();
		void createResetButton();
		void reset(CCObject* sender);
		virtual void draw();

		void addNewSpriteAtPosition(CCPoint p);
		void update(float dt);
		virtual void ccTouchesEnded(CCSet* touches, CCEvent* event);

		CREATE_FUNC(Box2DTestLayer);
	} ;


}



#endif
