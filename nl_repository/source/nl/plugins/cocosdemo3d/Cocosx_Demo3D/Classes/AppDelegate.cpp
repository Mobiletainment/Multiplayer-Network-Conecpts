#include "cocos2d.h"
#include "CCEGLView.h"
#include "AppDelegate.h"
#include "HelloWorldScene.h"

USING_NS_CC;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());


	CCSize screenSize = CCEGLView::sharedOpenGLView()->getFrameSize();

	CCSize designSize = CCSizeMake(480 * 2, 320 * 2);

	CCFileUtils* pFileUtils = CCFileUtils::sharedFileUtils();

	std::vector<std::string> searchPaths;
	searchPaths.push_back("../data/resources");
	searchPaths.push_back("../cocos_data/resources/samples/demo3d");
	searchPaths.push_back("../cocos_data/resources/samples");
	if (screenSize.height > 320)
	{
		CCSize resourceSize = CCSizeMake(960, 640);
		searchPaths.push_back("../cocos_data/resources/samples/hd");
		pDirector->setContentScaleFactor(resourceSize.height/designSize.height);
	}
	else	{
	}
	pFileUtils->setSearchPaths(searchPaths);


    // enable High Resource Mode(2x, such as iphone4) and maintains low resource on other devices.
//     pDirector->enableRetinaDisplay(true);

    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    CCScene *pScene = HelloWorld::scene();

    // run
    pDirector->runWithScene(pScene);
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();
}

void AppDelegate::applicationResized( int width, int height )
{
	CCDirector* director(CCDirector::sharedDirector());

	if(director->getRunningScene() != NULL)	{
		CCEGLView* glView = CCEGLView::sharedOpenGLView();
		glView->setFrameSize(static_cast<float>(width), static_cast<float>(height));
		glView->setDesignResolutionSize(480, 320, kResolutionNoBorder);
		director->setContentScaleFactor(0.9f);
		director->setContentScaleFactor(1.0f);
		glView->setDesignResolutionSize(static_cast<float>(width), static_cast<float>(height), kResolutionNoBorder);
	}

	// notify layers and scenes about the resize
	CCNotificationCenter* notificationCenter(CCNotificationCenter::sharedNotificationCenter());
	notificationCenter->postNotification("NL_NOTIFY_APPRESIZED", nullptr);
}
