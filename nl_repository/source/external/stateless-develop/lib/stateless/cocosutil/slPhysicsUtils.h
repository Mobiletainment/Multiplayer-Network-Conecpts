//!---------------------------------------------------------------------------
//! \file "slPhysicsUtils.h"
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
#ifndef __SLPHYSICSUTILS_H__
#define __SLPHYSICSUTILS_H__

class slCocosUtil_API Box2DUtils
{
public:
	static void setupSprite(CCPhysicsSprite* sprite, b2World* world, CCPoint p);
	static void destroySprite(CCPhysicsSprite* sprite, b2World* world);
};


#endif // __SLPHYSICSUTILS_H__
