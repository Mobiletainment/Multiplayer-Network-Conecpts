//!---------------------------------------------------------------------------
//! \file "slPhysicsUtils.cpp"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2012-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------


#include "stdAfx.h"

#include "slPhysicsUtils.h"
#include "slConstants.h"
#include "physics_nodes/CCPhysicsSprite.h"

// more infos on: http://www.iforce2d.net/b2dtut/bodies
// http://www.unagames.com/blog/daniele/2010/06/fixed-time-step-implementation-box2d?page=1


void Box2DUtils::setupSprite( CCPhysicsSprite* sprite, b2World* world, CCPoint p)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(p.x/PTM_RATIO, p.y/PTM_RATIO);
	CCNode* userData(sprite);
    bodyDef.userData = (void *)userData;
    
	b2Body *body = world->CreateBody(&bodyDef);

	// Define another box shape for our dynamic body.
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox((sprite->getContentSize().width * sprite->getScaleX()) / 2 / PTM_RATIO, 
		(sprite->getContentSize().height * sprite->getScaleY()) / 2 / PTM_RATIO);

	// Define the dynamic body fixture.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;    
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	body->CreateFixture(&fixtureDef);    

	sprite->setB2Body(body);
	sprite->setPTMRatio(PTM_RATIO);
	sprite->setPosition( ccp( p.x, p.y) );
}

void Box2DUtils::destroySprite(CCPhysicsSprite* sprite, b2World* world)
{
	world->DestroyBody(sprite->getB2Body());
}



