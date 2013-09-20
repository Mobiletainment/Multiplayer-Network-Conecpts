//!---------------------------------------------------------------------------
//! \file "nlPluginLayer.h"
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
#ifndef __NLPLUGINLAYER_H__
#define __NLPLUGINLAYER_H__

#include "nlPluginLayerContent.h"

namespace sl	{
	class NetworkState;
}

namespace nl	{

	class PluginLayer : public nl::ResizableLayer
	{
		SL_DECLARE_BASE(nl::ResizableLayer)
	public:

		virtual void onAppResized(CCObject* app);

		virtual bool init();  

		CREATE_FUNC(PluginLayer);

		virtual void onEnter();
		virtual void onExit();

		// CCTouchDelegate interface touch events
		virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
		virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

		virtual void update( float dt );

		CC_SYNTHESIZE_RETAIN(NetworkState*, _networkState, NetworkState)
		CC_SYNTHESIZE_RETAIN(PluginLayerContent*, _pluginLayerContent, PluginLayerContent)

		static void setModuleName( const char* moduleName );
		static const char* getModuleName( void );
	protected:
		PluginLayer();
		virtual ~PluginLayer();
	private:
		void updateWindowTitle() const;
		static SLAString _moduleName;
	};

	extern PluginLayerContent* createPluginLayerContent();

}

#endif // __NLPLUGINSCENE_H__
