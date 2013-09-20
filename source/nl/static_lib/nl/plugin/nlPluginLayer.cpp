//!---------------------------------------------------------------------------
//! \file "nlPluginLayer.cpp"
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
#include "nlPluginLayer.h"

namespace nl	{

	SLAString PluginLayer::_moduleName;

	PluginLayer::PluginLayer()
		:_networkState(nullptr)
		,_pluginLayerContent(nullptr)
	{
	}

	PluginLayer::~PluginLayer()	{
		setNetworkState(nullptr);
	}

	void PluginLayer::onAppResized(CCObject* app)	{
		SLBaseClass::onAppResized(app);
		CCRect visibleRect(nl::VisibleRect::getVisibleRect());
		CCDirector* director(CCDirector::sharedDirector());
		CCSize visibleSize = director->getVisibleSize();
		CCPoint origin = director->getVisibleOrigin();

		setContentSize(visibleRect.size);
		getPluginLayerContent()->onLayerResized(this);
	}

	void PluginLayer::onEnter()
	{
		SLBaseClass::onEnter();
	}

	void PluginLayer::onExit()
	{
		SLBaseClass::onExit();
	}

	void PluginLayer::updateWindowTitle() const	{
		// apply the module name to the main window
		CCEGLView* eglView(CCEGLView::sharedOpenGLView());
		eglView->setViewName(PluginLayer::getModuleName());
#ifdef _WIN32
		HWND windowHandle(eglView->getHWnd());

		const CCSize& frameSize = eglView->getFrameSize();
		if (frameSize.width > 0)
		{
			WCHAR wszBuf[MAX_PATH] = {0};
#ifdef _DEBUG
			char szBuf[MAX_PATH + 1];
			memset(szBuf, 0, sizeof(szBuf));
			snprintf(szBuf, MAX_PATH, "%s - %0.0fx%0.0f - %0.2f",
				PluginLayer::getModuleName(), frameSize.width, frameSize.height, eglView->getFrameZoomFactor());
			MultiByteToWideChar(CP_UTF8, 0, szBuf, -1, wszBuf, sizeof(wszBuf));
#else
			MultiByteToWideChar(CP_UTF8, 0, PluginLayer::getModuleName(), -1, wszBuf, sizeof(wszBuf));
#endif
			SetWindowTextW(windowHandle, wszBuf);
		}

#endif

	}


	// on "init" you need to initialize your instance
	bool PluginLayer::init()
	{
		// 1. super init first
		if ( !SLBaseClass::init() )
		{
			return false;
		}

		// add the 'global' network state to this layer 
		NetworkState* networkState(NetworkState::create());
		networkState->setFileName(PluginLayer::getModuleName());
		networkState->load();

		updateWindowTitle();

		// retrieve application options

		Poco::Util::LayeredConfiguration& appConfig(SL_PROCESS_APP()->config());
		SLAString numClients("");
		try
		{
			numClients = appConfig.getString("clients");
		}
		catch (...)
		{
		}

		// make this network state available
		// for external access
		setNetworkState(networkState);
		addChild(getNetworkState());

		// note: plugin layer content is a common CCObject
		//       the create function does not have a lot to do
		
		PluginLayerContent* content(createPluginLayerContent());
		if(content == nullptr)	{
			// sample content creation
			content = PluginLayerContent::create();
		}
		else	{
			if(numClients.empty() == false)	{
				content->setNumClients(CCString::create(numClients)->intValue());
			}
		}

		SL_PROCESS_APP()->log(ELogType_Message, "created plugin layer content object(%s)", content->getClassName());
		setPluginLayerContent(content);

		// to receive single touches
		// we could set these ...
		// but we don't want to
#if 0
		setTouchMode(kCCTouchesOneByOne);
		setTouchEnabled(true);
#endif

		// here the initial main workload is initiated
		getPluginLayerContent()->addContent(this);

		scheduleUpdate();
		return true;
	}

	void PluginLayer::update( float dt )	{
		SLBaseClass::update(dt);
	}

	bool PluginLayer::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
	{
#if 0
		CCLOG("PluginLayer::ccTouchBegan");
		// return true if we claim this touch
		// so we will receive further touch events
#endif
		return false;
	}

	void PluginLayer::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
	{
#if 0
		CCLOG("PluginLayer::ccTouchEnded");
#endif
	}

	void PluginLayer::setModuleName( const char* moduleName )
	{
		SLAString temp = moduleName;
		replaceString(temp, ".dll", "");
		replaceString(temp, "_debug", "");
		replaceString(temp, "_release", "");
		replaceString(temp, "_Debug", "");
		replaceString(temp, "_Release", "");
		_moduleName = temp;
	}

	const char* PluginLayer::getModuleName( void )
	{
		return _moduleName.c_str();
	}

}

