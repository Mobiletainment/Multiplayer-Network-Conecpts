//!---------------------------------------------------------------------------
//! \file "nlSingleLayerPluginContent.h"
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
#ifndef __NLSINGLELAYERPLUGINCONTENT_H__
#define __NLSINGLELAYERPLUGINCONTENT_H__

namespace nl	{

	//! @brief SingleLayerPluginContent features a single layer implementation
	//!
	class SingleLayerPluginContent : public PluginLayerContent	{
		SL_DECLARE_BASE(PluginLayerContent)
	public:
		SingleLayerPluginContent();
		virtual ~SingleLayerPluginContent();

		CREATE_FUNC(SingleLayerPluginContent)

		SL_IMPLEMENT_CLASSNAME(SingleLayerPluginContent)

		virtual bool init();

		virtual void addContent(CCLayer* parentLayer) SL_OVERRIDE;
		virtual void onLayerResized(CCLayer* parentLayer) SL_OVERRIDE;

		virtual void addRootMenuControls(CCArray* ctrlRowArray);

		virtual void addSubLayerContent(
			SLSize idx, 
			CCControlBase* twinLayer, 
			CCControlBase* leftLayer, 
			CCControlBase* rightLayer);

		virtual void onUIAction(CCObject *sender, CCControlEvent controlEvent);


		SL_SYNTHESIZE(GameplayLayer*, _gameplayLayer, GameplayLayer)

		

	};

}


#endif // __NLSingleLayerPluginContent_H__
