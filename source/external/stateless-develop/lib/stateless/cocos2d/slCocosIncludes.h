#pragma once
#ifndef __SLCOCOSINCLUDES_H__
#define __SLCOCOSINCLUDES_H__

//!---------------------------------------------------------------------------
//! \file "slCocosIncludes.h"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2012-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------



// cocos2d uses Box2D
#include "Box2D/Box2D.h"

#ifndef CC_ENABLE_BOX2D_INTEGRATION
#  define CC_ENABLE_BOX2D_INTEGRATION 1
#endif


// cocos2d-x includes
#pragma warning (push)
#pragma warning (disable : 4251)

#include "cocos2d.h"
#include "CCEGLView.h"
#include "support/CCNotificationCenter.h"

#include "cocos-ext.h"

#include "SimpleAudioEngine.h"


USING_NS_CC;
USING_NS_CC_EXT;
using namespace CocosDenshion;

#include "physics_nodes/CCPhysicsSprite.h"

#include "GUI/CCControlExtension/CCControlExtensions.h"
#include "GUI/CCControlExtension/CCControl.h"


#pragma warning (pop)


//-----------------------------------------------------------------------------

#include "slCocosMacros.h"


#endif  // __SLCOCOSINCLUDES_H__
