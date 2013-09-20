//!---------------------------------------------------------------------------
//! \file "stdafx.h"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2012-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------


// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

// slCore includes most commonly used stuff
#include <stateless/slCore.h>

// include common cocos2d headers and macros
#include <stateless/cocos2d/slCocosIncludes.h>

// include application classes
#include <stateless/application/slApplication.h>

// include cocos2d utility classes
#include <stateless/cocosutil/slCocosUtil.h>

// include framework classes
#include <stateless/framework/slFramework.h>

// include network classes
#include <stateless/network/slNetwork.h>

// include simulation classes
#include <stateless/simulation/slSimulation.h>

// allow namespace nl usage of
// - namespace sl
// - namespace sltl
namespace nl	{
	using namespace sl;
	using namespace sltl;
}

