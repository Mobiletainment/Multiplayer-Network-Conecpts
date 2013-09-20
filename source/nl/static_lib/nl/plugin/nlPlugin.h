#pragma once
#ifndef __NLPLUGIN_H__
#define __NLPLUGIN_H__
//!---------------------------------------------------------------------------
//! \file "nlPlugin.h"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2012-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------


#ifndef nlPlugin_EXPORTS
#  define nlPlugin_EXPORTS
#  define NL_DLL
#endif
#include "nlPluginAPI.h"

#if defined(nlPlugin_API)
#  pragma message( "--------------------------" )
#  pragma message( " Build Configuration: nlPlugin_API - declaring extern c functions" )
#  pragma message( "--------------------------" )
extern "C" 
{
	nlPlugin_API void SL_CRTDECL launch();

	nlPlugin_API cocos2d::CCLayer* SL_CRTDECL launchLayer();
}
#endif

//-----------------------------------------------------------------------------
#include "nlPluginLayer.h"
#include "nlPluginLayerContent.h"
#include "nlPeerOptionsUI.h"
#include "nlPeerOptionsDialog.h"

#endif  // __NLPLUGIN_H__
