//!---------------------------------------------------------------------------
//! \file "nlPluginLayerContent.cpp"
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

#include "nlPluginLayerContent.h"
#include "nlPluginOptionsDialog.h"
#include "nlPluginInfoDialog.h"
#include "nlPluginTasksDialog.h"

#include "nlPeerOptionsUI.h"

namespace nl	{

	PluginLayerContent::PluginLayerContent()
		:_quadLayer(nullptr)
		,_rootMenu(nullptr)
		,_numSubLayers(4)
		,_leftLayerClassName(nullptr)
		,_rightLayerClassName(nullptr)
		,_networkArchitecture(ENetworkArchitecture_UNKNOWN)
		,_pluginHasNetwork(false)
		,_pluginHasPhysics(false)
	{
	}

	PluginLayerContent::~PluginLayerContent()
	{
		setLeftLayerClassName(nullptr);
		setRightLayerClassName(nullptr);
	}

	bool PluginLayerContent::init()
	{
		return true;
	}

	void PluginLayerContent::onUIAction(CCObject *sender, CCControlEvent controlEvent)	{

		CCControlQuadLayer* quadLayer(getQuadLayer());

		bool dispatched(true);
		// determine the sender tag to dispatch the action
		const int tag(Dialog::getTag(sender));
		switch(tag)	{
		case SL_CTRLID_QUADLAYER_RECREATE:	{
			quadLayer->recreate(4);
			addSpecificContent();
			onLayerResized(dynamic_cast<CCLayer*>(quadLayer->getParent()));
							}
							break;
		case SL_CTRLID_QUADLAYER_RECREATE_TWINLAYER:	{
			quadLayer->recreate(1);
			addSpecificContent();
			onLayerResized(dynamic_cast<CCLayer*>(quadLayer->getParent()));
											}
											break;
		case SL_CTRLID_QUADLAYER_RECREATE_TWO_TWINLAYERS:	{
			quadLayer->recreate(2);
			addSpecificContent();
			onLayerResized(dynamic_cast<CCLayer*>(quadLayer->getParent()));
														}
														break;
		case SL_CTRLID_QUADLAYER_RECREATE_THREE_TWINLAYERS:	{
			quadLayer->recreate(3);
			addSpecificContent();
			onLayerResized(dynamic_cast<CCLayer*>(quadLayer->getParent()));
															}
															break;
		case SL_CTRLID_OPTIONS:
			{
				setPluginHasNetwork(PeerNode::getPeerNodesCount() > 0);
				PluginOptionsDialog::setPluginLayerContent(this);
				PluginOptionsDialog::create()->show();
				PluginOptionsDialog::setPluginLayerContent(nullptr);
			}
			break;
		case SL_CTRLID_INFO:
			{
				PluginInfoDialog::create()->show();
			}
			break;
		case SL_CTRLID_TASKS:
			{
				PluginTasksDialog::create()->show();
			}
			break;
		default:	{
			dispatched = false;
					}
					break;
		}
	}

	void PluginLayerContent::addRootMenuControls(CCArray* ctrlRowArray)	{
		ctrlRowArray->addObject(createRootButton("Recreate Quadlayer", SL_CTRLID_QUADLAYER_RECREATE));
		ctrlRowArray->addObject(createRootButton("Recreate 1 Twinlayer", SL_CTRLID_QUADLAYER_RECREATE_TWINLAYER));
		ctrlRowArray->addObject(createRootButton("Recreate 2 Twinlayers", SL_CTRLID_QUADLAYER_RECREATE_TWO_TWINLAYERS));
		ctrlRowArray->addObject(createRootButton("Recreate 3 Twinlayers", SL_CTRLID_QUADLAYER_RECREATE_THREE_TWINLAYERS));
	}

	void PluginLayerContent::addRootLayerContent()	{
		CCControlQuadLayer* quadLayer(getQuadLayer());
		const CCSize& contentSize(quadLayer->getContentSize());

		CCSize btnContainerSize(CCSizeMake(contentSize.width,0));
		CCArray* ctrlRowArray = CCArray::create();

		CCControlBase* leftSpace(CCControlBase::create());
		leftSpace->setPreferredSize(CCSizeMake(60,NL_MAINLAYER_FRAME_HEIGHT - quadLayer->getHorizontalMargin() * 2));
		leftSpace->needsLayout();
		ctrlRowArray->addObject(leftSpace);

		ctrlRowArray->addObject(createRootButton("Options", SL_CTRLID_OPTIONS));

		addRootMenuControls(ctrlRowArray);

		CCControlRow* btnContainer = ControlUtils::createControlRow(ctrlRowArray,btnContainerSize);
		quadLayer->addChild(btnContainer);
		_rootMenu = btnContainer;
	}

	void PluginLayerContent::addSubLayerContent( 
		SLSize idx, 
		CCControlBase* twinLayer, 
		CCControlBase* leftLayer, 
		CCControlBase* rightLayer )
	{
		CCSize ctrlsPreferredSize(CCSizeMake(0,0));
		CCArray* ctrls;
		CCControlColumn* ctrlContainer;
		// left layer setup
		ctrls = CCArray::create();
		ctrls->addObject(ControlUtils::createLabel("not implemented yet"));
		ctrlContainer = ControlUtils::createControlColumn(ctrls, ctrlsPreferredSize);
		leftLayer->addChild(ctrlContainer);

		// right layer setup
		ctrls = CCArray::create();
		ctrls->addObject(ControlUtils::createLabel("not implemented yet"));
		ctrlContainer = ControlUtils::createControlColumn(ctrls, ctrlsPreferredSize);
		rightLayer->addChild(ctrlContainer);
	}

	void PluginLayerContent::recreateSubLayers(SLSize numSublayers)	{
		CCControlQuadLayer* quadLayer(getQuadLayer());
		if(quadLayer != nullptr)	{
			// before we do this
			// shutdown all peers to avoid any cleanup hazle
			PeerNode::shutDownAllPeers();

			// now we should be able to remove all sub layers
			// in case not ... this is a nice debug entry point
			quadLayer->recreate(numSublayers);
			addSpecificContent();
			onLayerResized(dynamic_cast<CCLayer*>(quadLayer->getParent()));
		}
	}

	void PluginLayerContent::addSpecificContent()	{
		CCControlQuadLayer* quadLayer(getQuadLayer());

		for(SLSize i(0); i < 4; ++i)	{
			// the twin layer is the master to add the peer node to
			CCControlTwinLayer* twinLayer(dynamic_cast<CCControlTwinLayer*>(quadLayer->getSubLayer(i)));
			if(twinLayer != nullptr)	{
				// left and right child layer
				CCControlBase* leftLayer = twinLayer->getChildLayer(0);
				CCControlBase* rightLayer = twinLayer->getChildLayer(1);

				addSubLayerContent(i, twinLayer, leftLayer, rightLayer);

				leftLayer->layoutChildren(ELayoutMode_topLeftDown);
				rightLayer->layoutChildren(ELayoutMode_topLeftDown);
			}
		}
	}

	void PluginLayerContent::addContent(CCLayer* parentLayer)	{
		const CCSize& contentSize(parentLayer->getContentSize());
		// determine the size for a single sublayer

		const float subLayerWidth((contentSize.width - NL_APPLAYER_FRAME_WIDTH * 2.0f));
		const float subLayerHeight((contentSize.height - NL_APPLAYER_FRAME_WIDTH));

		CCSize subLayerSize(CCSizeMake(subLayerWidth, subLayerHeight));

		_quadLayer = ControlUtils::createQuadControl(
			subLayerSize, 
			getNumSubLayers(),
			getLeftLayerClassName(),
			getRightLayerClassName());

		CCPoint layerPosition(NL_APPLAYER_FRAME_WIDTH, NL_APPLAYER_FRAME_WIDTH);
		_quadLayer->setPosition(layerPosition);
		addRootLayerContent();
		addSpecificContent();

		parentLayer->addChild(_quadLayer,0);
	}

	void PluginLayerContent::onLayerResized(CCLayer* parentLayer)	{
		if(_quadLayer == nullptr)	{
			return;
		}
		const CCSize& contentSize(parentLayer->getContentSize());
		// determine the size for a single sublayer
		const float subLayerWidth((contentSize.width - NL_APPLAYER_FRAME_WIDTH * 2.0f));
		const float subLayerHeight((contentSize.height - NL_APPLAYER_FRAME_WIDTH));

		CCPoint layerPosition(NL_APPLAYER_FRAME_WIDTH, NL_APPLAYER_FRAME_WIDTH);
		CCSize subLayerSize(CCSizeMake(subLayerWidth, subLayerHeight));
		_quadLayer->setPosition(layerPosition);
		_quadLayer->setPreferredSize(subLayerSize);
		_quadLayer->needsLayout();

		if(_rootMenu != nullptr)	{
			_rootMenu->setPreferredSize(CCSizeMake(subLayerSize.width, 0));
			// TODO: check protected needsLayout
			_rootMenu->needsLayout();
		}

		CCControlQuadLayer* quadLayer(_quadLayer);
		for(SLSize i(0); i < 4; ++i)	{
			CCControlTwinLayer* twinLayer(dynamic_cast<CCControlTwinLayer*>(quadLayer->getSubLayer(i)));
			if(twinLayer != nullptr)	{
				// left and right child layer
				CCControlBase* leftLayer = twinLayer->getChildLayer(0);
				CCControlBase* rightLayer = twinLayer->getChildLayer(1);

				leftLayer->layoutChildren(ELayoutMode_topLeftDown);
				rightLayer->layoutChildren(ELayoutMode_topLeftDown);
			}
		}
	}

	CCControlButton* PluginLayerContent::createRootButton( const char* btnTitle, int btnTag )
	{
		CCControlButton* ctrlBtn = nl::ControlUtils::createButton(btnTitle);
		ctrlBtn->setTag(btnTag);
		ctrlBtn->addTargetWithActionForControlEvents(
			this, 
			cccontrol_selector(PluginLayerContent::onUIAction), 
			CCControlEventTouchUpInside);
		return ctrlBtn;
	}

}

