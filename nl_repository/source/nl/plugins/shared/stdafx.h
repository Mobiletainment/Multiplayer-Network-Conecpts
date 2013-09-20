#pragma once
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


// allow namespace nl usage of
// - namespace stl
// - namespace sl
// - namespace sltl
// - cocos namespaces
// - namespace RakNet
namespace nl	{
	using namespace std;

	using namespace sl;
	using namespace sltl;

	USING_NS_CC;
	USING_NS_CC_EXT;

	using namespace RakNet;

}

#include <nl/plugin/nlPlugin.h>
