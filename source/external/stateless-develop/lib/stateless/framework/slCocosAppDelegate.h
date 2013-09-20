//!---------------------------------------------------------------------------
//! \file "slCocosAppDelegate.h"
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
#ifndef  __SLCOCOSAPPDELEGATE_H__
#define  __SLCOCOSAPPDELEGATE_H__

#include "slPluginInterface.h"

namespace sl	{
/**
@brief    The cocos2d Application.

The reason for implement as private inheritance is to hide some interface call by CCDirector.
*/
class  slFramework_API CocosAppDelegate : private cocos2d::CCApplication
{
public:

	static int launch( nlFunc_launchLayer* launchLayerFunc );

    CocosAppDelegate();
    virtual ~CocosAppDelegate();

    /**
    @brief    Implement CCDirector and CCScene init code here.
    @return true    Initialize success, app continue.
    @return false   Initialize failed, app terminate.
    */
    virtual bool applicationDidFinishLaunching();

    /**
    @brief  The function be called when the application enter background
    @param  the pointer of the application
    */
    virtual void applicationDidEnterBackground();

    /**
    @brief  The function be called when the application enter foreground
    @param  the pointer of the application
    */
    virtual void applicationWillEnterForeground();

    /**
     @brief called on windows if the window size changes
     */
	virtual void applicationResized(int width, int height);

	CC_SYNTHESIZE(nlFunc_launchLayer*, _launchLayerFunc, LauncLayerFunc)

};

}

#endif // __SLCOCOSAPPDELEGATE_H__

