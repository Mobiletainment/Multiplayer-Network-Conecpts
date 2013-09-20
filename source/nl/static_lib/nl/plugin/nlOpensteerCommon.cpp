//!---------------------------------------------------------------------------
//! \file "nlOpensteerCommon.cpp"
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

#include <stateless/simulation/slSimulation.h>

namespace {
	bool initializeGlobals( void )
	{
		OpenSteer::GlobalData::_SDMInitApp( NULL );
		static OpenSteer::OpenGLRenderer renderer;
		OpenSteer::GlobalData::getInstance()->setRenderer( &renderer );
		return true;
	}

	bool globalsInitialized(initializeGlobals());

} // anonymous namespace

//-----------------------------------------------------------------------------

namespace OpenSteer	{
	void handleGlobalDataInstanceFailure( void )
	{
		if( ( GlobalData::hasInstance() == false ) )
		{
			// the open steer sample is heavily working
			// with global data so initialization
			// is very random
			initializeGlobals();
		}
	}

	//-----------------------------------------------------------------------------
	// accessors for GL window dimensions
	float extern_drawGetWindowHeight (void) 
	{
		CCDirector* director(CCDirector::sharedDirector());
		return director->getVisibleSize().height;
	}


	float extern_drawGetWindowWidth  (void) 
	{
		CCDirector* director(CCDirector::sharedDirector());
		return director->getVisibleSize().width;
	}

}


