//!---------------------------------------------------------------------------
//! \file "nlPluginLayerContent.h"
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
#ifndef __NLPLUGINLAYERCONTENT_H__
#define __NLPLUGINLAYERCONTENT_H__

namespace nl	{

	class AbstractPluginLayerContent
	{
	public:
		SL_DECLARE_ABSTRACT_CLASSNAME

	protected:
		virtual ~AbstractPluginLayerContent()	{

		}
	};


	class PluginLayerContent : public CCObject, public AbstractPluginLayerContent
	{
		SL_DECLARE_BASE(CCObject)

	public:

		CREATE_FUNC(PluginLayerContent)

		SL_IMPLEMENT_CLASSNAME(PluginLayerContent)

		virtual void addRootLayerContent();
		virtual void addRootMenuControls(CCArray* ctrlRowArray);

		CCControlButton* createRootButton( const char* btnTitle, int btnTag );

		virtual void addSubLayerContent(
			SLSize idx, 
			CCControlBase* twinLayer, 
			CCControlBase* leftLayer, 
			CCControlBase* rightLayer);

		virtual void addContent(CCLayer* parentLayer);

		virtual void onLayerResized(CCLayer* parentLayer);

		virtual void onUIAction(CCObject *sender, CCControlEvent controlEvent);

		void recreateSubLayers(SLSize numSublayers);

		CC_SYNTHESIZE(SLSize, _numSubLayers, NumSubLayers)

		CC_SYNTHESIZE_RETAIN(CCString*, _leftLayerClassName, LeftLayerClassName)
		CC_SYNTHESIZE_RETAIN(CCString*, _rightLayerClassName, RightLayerClassName)

		SL_SYNTHESIZE(ENetworkArchitecture, _networkArchitecture, NetworkArchitecture)
		SL_SYNTHESIZE(int, _numClients, NumClients)

		SL_SYNTHESIZE(bool, _pluginHasNetwork, PluginHasNetwork)
		SL_SYNTHESIZE(bool, _pluginHasPhysics, PluginHasPhysics)

	protected:
		PluginLayerContent();
		virtual ~PluginLayerContent();

		virtual bool init();

		CCControlQuadLayer* getQuadLayer() const;

		void addSpecificContent();

	private:
		CCControlQuadLayer* _quadLayer;
		CCControlRow* _rootMenu;
	};

	SL_INLINE
	CCControlQuadLayer* PluginLayerContent::getQuadLayer() const	{
		return _quadLayer;
	}


}

#endif // __NLPLUGINLAYERCONTENT_H__
