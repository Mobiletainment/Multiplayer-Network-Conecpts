//!---------------------------------------------------------------------------
//! \file "nlPluginExports.cpp"
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

// this is the dynamic lib function we export
nlPlugin_API cocos2d::CCLayer* SL_CRTDECL launchLayer()	{
	cocos2d::CCLayer* layer(nl::PluginLayer::create());
	return layer;
}

