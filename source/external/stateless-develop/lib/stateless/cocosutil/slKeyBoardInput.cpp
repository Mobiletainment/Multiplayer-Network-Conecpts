//!---------------------------------------------------------------------------
//! \file "slKeyBoardInput.cpp"
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
#include "slKeyBoardInput.h"
#include "slCocosUtilMacros.h"

#define NL_MAX_KEYS 0xFF
namespace	{
	bool g_keyState[NL_MAX_KEYS];
}

//-----------------------------------------------------------------------------
bool handleKeyDown( WPARAM wParam )
{
	bool keyChange(false);
	if(g_keyState[wParam] != true)	{
		g_keyState[wParam] = true;
		keyChange = true;
	}
	return keyChange;
}

//-----------------------------------------------------------------------------
bool handleKeyUp( WPARAM wParam )
{
	bool keyChange(false);
	if(g_keyState[wParam] != false)	{
		g_keyState[wParam] = false;
		keyChange = true;
	}
	return keyChange;
}

//-----------------------------------------------------------------------------
void CCKeyBoardHook( UINT message,WPARAM wParam,LPARAM lParam )
{
	unsigned char key(static_cast<unsigned char>(wParam));
	bool sendUpdate(false);
	bool keyDown(false);
	bool keyUp(false);
	switch( message )
	{
	case WM_KEYDOWN:
		keyDown = sendUpdate = handleKeyDown( wParam );
		break;
	case WM_KEYUP:
		keyUp = sendUpdate = handleKeyUp( wParam );
		break;
	case WM_CHAR:
		// Deliberately empty - all handled through key up and down events
		break;
	default:
		// Not expected to get here!!
		CC_ASSERT( false );
		break;
	}

	if ( sendUpdate )
	{
		// notify layers and scenes about the resize
		CCObject* keyObject(new CCObject());
		keyObject->autorelease();
		keyObject->m_uID = key;

		CCNotificationCenter* notificationCenter(CCNotificationCenter::sharedNotificationCenter());
		if(keyDown)	{
			notificationCenter->postNotification(NL_NOTIFY_KEY_DOWN, keyObject);
		}
		if(keyUp)	{
			notificationCenter->postNotification(NL_NOTIFY_KEY_UP, keyObject);
		}

//		CCLOG("CCKeyBoardHook");
//		const time_t    theTime=time(NULL);
//		const double    timestamp=(double)theTime / 100.0;
//		pAccelerometer->update( g_accelX,g_accelY,g_accelZ,timestamp );
	}

}


void KeyBoardInput::setup( void )
{
	for(SLSize i(0); i < NL_MAX_KEYS; ++i)	{
		g_keyState[i] = 0;
	}
	CCEGLView::sharedOpenGLView()->setAccelerometerKeyHook( &CCKeyBoardHook );
}


/*
 * VK_0 - VK_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
 * VK_A - VK_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
 */


bool KeyBoardInput::isKeyDown( char key )	{
	return g_keyState[key];
}
