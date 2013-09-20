#include "AppDelegate.h"

#include "cocos2d.h"
#include "controller.h"
#include "SimpleAudioEngine.h"
#include "cocos-ext.h"
#include "CCArmature/utils/CCArmatureDataManager.h"

USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
//    SimpleAudioEngine::end();
	cocos2d::extension::CCArmatureDataManager::purgeArmatureSystem();
}

bool AppDelegate::applicationDidFinishLaunching()
{
	// As an example, load config file
	// XXX: This should be loaded before the Director is initialized,
	// XXX: but at this point, the director is already initialized
	CCConfiguration::sharedConfiguration()->loadConfigFile("../data/configs/config-example.plist");

    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());

    CCSize screenSize = CCEGLView::sharedOpenGLView()->getFrameSize();

    CCSize designSize = CCSizeMake(480, 320);

    CCFileUtils* pFileUtils = CCFileUtils::sharedFileUtils();
    
	std::vector<std::string> searchPaths;
    if (screenSize.height > 320)
    {
        CCSize resourceSize = CCSizeMake(960, 640);
        searchPaths.push_back("../cocos_data/resources/samples/hd");
        pDirector->setContentScaleFactor(resourceSize.height/designSize.height);
    }
	else	{
	}
	searchPaths.push_back("../cocos_data/resources/samples");
	searchPaths.push_back("../data/resources");
	searchPaths.push_back("../data/resources/nl");
	pFileUtils->setSearchPaths(searchPaths);

    CCEGLView::sharedOpenGLView()->setDesignResolutionSize(designSize.width, designSize.height, kResolutionNoBorder);

    CCScene * pScene = CCScene::create();
    CCLayer * pLayer = new TestController();
    pLayer->autorelease();

    pScene->addChild(pLayer);
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}

void AppDelegate::applicationResized( int width, int height )
{

}
