//!---------------------------------------------------------------------------
//! \file "CCControlTwinLayer.cpp"
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
#include "CCControlTwinLayer.h"
#include "ControlFactory.h"
#include "../slControlUtils.h"


namespace sl	{



	CCControlTwinLayer::CCControlTwinLayer()
		:_leftLayerClassName(nullptr)
		,_rightLayerClassName(nullptr)
	{
		_childLayer[0] =
		_childLayer[1] = nullptr;
	}

	CCControlTwinLayer::~CCControlTwinLayer()
	{
		setLeftLayerClassName(nullptr);
		setRightLayerClassName(nullptr);
	}

	void CCControlTwinLayer::needsLayout()
	{
		SLBaseClass::needsLayout();

		if(_childLayer[0] != nullptr)	{
			CCControlBase* subLayer(this);
			const CCSize& contentSize(subLayer->getContentSize());
			const float subLayerWidth((contentSize.width - NL_SUBLAYER_FRAME_WIDTH * 2.0f));
			const float subLayerHeight((contentSize.height - NL_SUBLAYER_FRAME_WIDTH * 2.0f));

			CCSize subLayerSize(CCSizeMake(subLayerWidth, subLayerHeight));
			CCPoint layerPosition(CCPointMake(0,0));
			layerPosition.x += NL_SUBLAYER_FRAME_WIDTH;
			layerPosition.y += NL_SUBLAYER_FRAME_WIDTH;

			CCSize childLayerSize(subLayerSize);
			childLayerSize.width = (subLayerWidth > 260.0f ? 250.0f : subLayerWidth);
			for(SLSize i(0); i < 2; ++i)	{
				// special case ...
				if(_childLayer[i]->isVisible() == false)	{
					childLayerSize.width = std::max(5.0f,subLayerWidth);
				}
				else	{
					_childLayer[i]->setPosition(layerPosition);
					_childLayer[i]->setPreferredSize(childLayerSize);
					_childLayer[i]->needsLayout();

					layerPosition.x += childLayerSize.width;
					childLayerSize.width = std::max(5.0f,subLayerWidth - childLayerSize.width);
				}
			}
		}
	}

	void CCControlTwinLayer::setContentSize(const CCSize& contentSize)	{
		SLBaseClass::setContentSize(contentSize);
	}

	void CCControlTwinLayer::addSubLayerContent()
	{
		CCControlBase* subLayer(this);
		const CCSize& contentSize(subLayer->getContentSize());
		const float subLayerWidth((contentSize.width - NL_SUBLAYER_FRAME_WIDTH * 2.0f));
		const float subLayerHeight((contentSize.height - NL_SUBLAYER_FRAME_WIDTH * 2.0f));

		CCSize subLayerSize(CCSizeMake(subLayerWidth, subLayerHeight));
		CCPoint layerPosition(CCPointMake(0,0));
		layerPosition.x += NL_SUBLAYER_FRAME_WIDTH;
		layerPosition.y += NL_SUBLAYER_FRAME_WIDTH;

		CCSize childLayerSize(subLayerSize);
		childLayerSize.width = subLayerWidth * 1.0f/3.0f;
		for(SLSize i(0); i < 2; ++i)	{
			const char* layerClassName(CCControlBase::staticClassName());
			if(i == 0)	{
				layerClassName = getLeftLayerClassName() ? getLeftLayerClassName()->getCString() : layerClassName;
			}
			else if(i == 1)	{
				layerClassName = getRightLayerClassName() ? getRightLayerClassName()->getCString() : layerClassName;
			}
			// we create the subview sprite here
			CCScale9Sprite* backgroundSprite(ControlUtils::createSubViewBackground());
			_childLayer[i] = ControlFactory::createControl(layerClassName, childLayerSize, backgroundSprite);
			_childLayer[i]->setPosition(layerPosition);

			subLayer->addChild(_childLayer[i],0);

			layerPosition.x += childLayerSize.width;
			childLayerSize.width = subLayerWidth * 2.0f/3.0f;
		}
	}

}


