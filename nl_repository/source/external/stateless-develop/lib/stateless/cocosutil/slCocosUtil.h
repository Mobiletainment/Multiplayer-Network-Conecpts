#pragma once
#ifndef __SLCOCOSUTIL_H__
#define __SLCOCOSUTIL_H__

//!---------------------------------------------------------------------------
//! \file "slCocosUtil.h"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2012-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------


// include common cocos2d headers and macros
#include <stateless/cocos2d/slCocosIncludes.h>


#include "slCocosUtilAPI.h"

USING_NS_CC;
USING_NS_CC_EXT;

#include "slCCInt64.h"
#include "slCCSmartPointer.h"

#include "slNotification.h"

#include "slKeyBoardInput.h"
#include "slPhysicsUtils.h"
#include "slVisibleRect.h"
#include "slControlUtils.h"
#include "slResizeableLayer.h"
#include "slValueConversion.h"
#include "slAbstractNode.h"

#include "components/slComponent.h"

#include "controls/CCControlBase.h"
#include "controls/ControlFactory.h"

#include "actor/slAbstractActor.h"
#include "actor/slActorComponent.h"
#include "actor/slActorNode.h"

#include "logic/slPersistentState.h"

#include "objectstore/slDictionary.h"
#include "objectstore/slPersistentDictionary.h"


#include "json/CCJSONConverter.h"

//-----------------------------------------------------------------------------

#endif  // __SLCOCOSUTIL_H__
