//!---------------------------------------------------------------------------
//! \file "slPluginInterface.h"
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
#ifndef __SLPLUGININTERFACE_H__
#define __SLPLUGININTERFACE_H__

namespace cocos2d	{
	class CCLayer;
}	

typedef  void (nlFunc_launch)( void );
typedef  cocos2d::CCLayer* (nlFunc_launchLayer)( void );



#endif
