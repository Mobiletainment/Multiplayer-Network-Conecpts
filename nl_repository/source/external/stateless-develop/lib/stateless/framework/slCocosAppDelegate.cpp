//!---------------------------------------------------------------------------
//! \file "slCocosAppDelegate.cpp"
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
#include "slCocosAppDelegate.h"

#include <stateless/cocosutil/slCocosUtil.h>

using namespace std;

CCSize g_frameSize(1100,660);


/* For demonstrating using one design resolution to match different resources,
   or one resource to match different design resolutions.

   [Situation 1] Using one design resolution to match different resources.
     Please look into Appdelegate::applicationDidFinishLaunching.
     We check current device frame size to decide which resource need to be selected.
     So if you want to test this situation which said in title '[Situation 1]',
     you should change ios simulator to different device(e.g. iphone, iphone-retina3.5, iphone-retina4.0, ipad, ipad-retina),
     or change the window size in "proj.XXX/main.cpp" by "CCEGLView::setFrameSize" if you are using win32 or linux plaform
     and modify "proj.mac/AppController.mm" by changing the window rectangle.

   [Situation 2] Using one resource to match different design resolutions.
     The coordinates in your codes is based on your current design resolution rather than resource size.
     Therefore, your design resolution could be very large and your resource size could be small.
     To test this, just define the marco 'TARGET_DESIGN_RESOLUTION_SIZE' to 'DESIGN_RESOLUTION_2048X1536'
     and open iphone simulator or create a window of 480x320 size.

   [Note] Normally, developer just need to define one design resolution(e.g. 960x640) with one or more resources.
 */

#define DESIGN_RESOLUTION_480X320    0
#define DESIGN_RESOLUTION_1024X768   1
#define DESIGN_RESOLUTION_2048X1536  2

/* If you want to switch design resolution, change next line */
#define TARGET_DESIGN_RESOLUTION_SIZE  DESIGN_RESOLUTION_480X320

typedef struct tagResource
{
    cocos2d::CCSize size;
    char directory[100];
} Resource;

static Resource smallResource  =  { cocos2d::CCSizeMake(480, 320),   "Resources/iphone" };
static Resource mediumResource =  { cocos2d::CCSizeMake(1024, 768),  "Resources/ipad"   };
static Resource largeResource  =  { cocos2d::CCSizeMake(2048, 1536), "Resources/ipadhd" };

#if (TARGET_DESIGN_RESOLUTION_SIZE == DESIGN_RESOLUTION_480X320)
static cocos2d::CCSize designResolutionSize = cocos2d::CCSizeMake(480, 320);
#elif (TARGET_DESIGN_RESOLUTION_SIZE == DESIGN_RESOLUTION_1024X768)
static cocos2d::CCSize designResolutionSize = cocos2d::CCSizeMake(1024, 768);
#elif (TARGET_DESIGN_RESOLUTION_SIZE == DESIGN_RESOLUTION_2048X1536)
static cocos2d::CCSize designResolutionSize = cocos2d::CCSizeMake(2048, 1536);
#else
#error unknown target design resolution!
#endif

// The font size 24 is designed for small resolution, so we should change it to fit for current design resolution
#define TITLE_FONT_SIZE  (cocos2d::CCEGLView::sharedOpenGLView()->getDesignResolutionSize().width / smallResource.size.width * 24)


// create the application instance
namespace sl	{


	int CocosAppDelegate::launch( nlFunc_launchLayer* launchLayerFunc )
	{
		int returnCode(0);
		static bool launched(false);
		if(launched == false)	{
			launched = true;
			CocosAppDelegate app;
			app.setLauncLayerFunc(launchLayerFunc);
			CCEGLView* eglView(CCEGLView::sharedOpenGLView());
			eglView->setViewName("Cocos2D-x");
			eglView->setFrameSize(g_frameSize.width, g_frameSize.height);
			KeyBoardInput::setup();
			returnCode = CCApplication::sharedApplication()->run();
		}
		else	{
			SL_PROCESS_APP()->log(sl::ELogType_Info, "right now you can not launch cocos2d-x a second time");
		}
		return returnCode;
	}


	//-----------------------------------------------------------------------------
	CocosAppDelegate::CocosAppDelegate()
		:_launchLayerFunc(nullptr)
	{
		SL_PROCESS_APP()->log(sl::ELogType_Info, "appname %s", SL_PROCESS_APP()->name());
	}

	CocosAppDelegate::~CocosAppDelegate() 
	{
	}

	void CocosAppDelegate::applicationResized(int width, int height)
	{
		sl::VisibleRect::invalidate();

		CCDirector* director(CCDirector::sharedDirector());

		if(director->getRunningScene() != NULL)	{
			CCEGLView* glView = CCEGLView::sharedOpenGLView();
			glView->setFrameSize(static_cast<float>(width), static_cast<float>(height));
			director->setContentScaleFactor(0.9f);
			director->setContentScaleFactor(1.0f);
			glView->setDesignResolutionSize(static_cast<float>(width), static_cast<float>(height), kResolutionNoBorder);
		}

		// notify layers and scenes about the resize
		CCNotificationCenter* notificationCenter(CCNotificationCenter::sharedNotificationCenter());
		notificationCenter->postNotification(SL_NOTIFY_APPRESIZED, nullptr);
	}

	bool CocosAppDelegate::applicationDidFinishLaunching() {

		// initialize director
		CCDirector* director = CCDirector::sharedDirector();
		CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

		director->setOpenGLView(pEGLView);

		designResolutionSize = g_frameSize;

		// Set the design resolution
		pEGLView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionNoBorder);

		CCSize frameSize = pEGLView->getFrameSize();

		vector<string> searchPath;

		// In this demo, we select resource according to the frame's height.
		// If the resource size is different from design resolution size, you need to set contentScaleFactor.
		// We use the ratio of resource's height to the height of design resolution,
		// this can make sure that the resource's height could fit for the height of design resolution.

		// if the frame's height is larger than the height of medium resource size, select large resource.
		if (frameSize.height > mediumResource.size.height)
		{
			searchPath.push_back(largeResource.directory);
		}
		// if the frame's height is larger than the height of small resource size, select medium resource.
		else if (frameSize.height > smallResource.size.height)
		{
			searchPath.push_back(mediumResource.directory);
		}
		// if the frame's height is smaller than the height of medium resource size, select small resource.
		else
		{
			searchPath.push_back(smallResource.directory);
		}

		director->setContentScaleFactor(1.0f);

		searchPath.push_back("../data/resources");
		searchPath.push_back("../data/resources/nl");
		searchPath.push_back("../data/resources/nl/multiplayer_art");

		// set searching path
		CCFileUtils::sharedFileUtils()->setSearchPaths(searchPath);

		// turn on display FPS
		director->setDisplayStats(true);

		// set FPS. the default value is 1.0/60 if you don't call this
		director->setAnimationInterval(1.0 / 60);

		// create a scene. it's an autorelease object
		CCScene *scene(nullptr);
		if(_launchLayerFunc)	{
			scene = CCScene::create();
			CCLayer* layer(_launchLayerFunc());
			scene->addChild(layer);
			director->runWithScene(scene);
			return true;
		}
		return false;
	}

	// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
	void CocosAppDelegate::applicationDidEnterBackground() {
		CCDirector::sharedDirector()->stopAnimation();

		// if you use SimpleAudioEngine, it must be pause
		SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	}

	// this function will be called when the app is active again
	void CocosAppDelegate::applicationWillEnterForeground() {
		CCDirector::sharedDirector()->startAnimation();

		// if you use SimpleAudioEngine, it must resume here
		SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
	}

}
