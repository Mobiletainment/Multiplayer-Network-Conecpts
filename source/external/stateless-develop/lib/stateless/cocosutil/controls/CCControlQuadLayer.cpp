//!---------------------------------------------------------------------------
//! \file "CCControlQuadLayer.cpp"
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
#include "CCControlQuadLayer.h"
#include "../slControlUtils.h"
#include "../slVisibleRect.h"


namespace sl	{

	CCControlQuadLayer::CCControlQuadLayer()
		:_numSubLayers(4)
		,_leftLayerClassName(nullptr)
		,_rightLayerClassName(nullptr)

	{
		for(SLSize i(0); i < 4; ++i)	{
			_subLayer[i] = nullptr;
		}
	}

	CCControlQuadLayer::~CCControlQuadLayer()
	{
		for(SLSize i(0); i < 4; ++i)	{
			_subLayer[i] = nullptr;
		}
		setLeftLayerClassName(nullptr);
		setRightLayerClassName(nullptr);
	}

	void CCControlQuadLayer::needsLayout()
	{
		SLBaseClass::needsLayout();

		if(_subLayer[0] != nullptr)	{
			CCControlBase* layer(this);

			const SLSize numSubLayers = sl::min(SLSize(4), getNumSubLayers());

			const CCSize& contentSize(layer->getContentSize());

			// determine the size for a single sublayer
			const float sideFrameWidth(NL_SUBLAYER_FRAME_WIDTH);
			const float subLayerWidth((contentSize.width - sideFrameWidth * 2.0f) * 0.5f);
			const float subLayerHeight((contentSize.height - NL_MAINLAYER_FRAME_HEIGHT) * 0.5f);

			CCSize subLayerSize(CCSizeMake(subLayerWidth, subLayerHeight));

			// layout
			NodeRect layerRect(NodeRect(layer,true));
			CCPoint layerPosition(layerRect.leftTop());
			layerPosition.x += sideFrameWidth;
			layerPosition.y -= subLayerHeight + NL_MAINLAYER_FRAME_HEIGHT;


			switch(numSubLayers)	{
			case(1):	{
				SLSize i(0);

					layerPosition.y += NL_MAINLAYER_FRAME_HEIGHT;
					layerPosition.y -= subLayerSize.height;

					_subLayer[i]->setPosition(layerPosition);
					_subLayer[i]->setPreferredSize(CCSizeMake(subLayerSize.width * 2, subLayerSize.height * 2));
					_subLayer[i]->needsLayout();

						}
						break;
			case(2):	{
				for(SLSize i(0); i < 2; i=i+2)	{

					layerPosition.y += NL_MAINLAYER_FRAME_HEIGHT;
					layerPosition.y -= subLayerSize.height;

					_subLayer[i]->setPosition(layerPosition);
					_subLayer[i]->setPreferredSize(CCSizeMake(subLayerSize.width, subLayerSize.height * 2));
					_subLayer[i]->needsLayout();

					layerPosition.x += subLayerWidth;
					_subLayer[i+1]->setPosition(layerPosition);
					_subLayer[i+1]->setPreferredSize(CCSizeMake(subLayerSize.width, subLayerSize.height * 2));
					_subLayer[i+1]->needsLayout();
				}
						}
						break;
			case(3):	{
				SLSize i(0);
				for(i; i < 2; i = i + 2)	{
					layerPosition.y += NL_MAINLAYER_FRAME_HEIGHT;

					_subLayer[i]->setPosition(layerPosition);
					_subLayer[i]->setPreferredSize(subLayerSize);
					_subLayer[i]->needsLayout();

					layerPosition.x += subLayerWidth;
					_subLayer[i+1]->setPosition(layerPosition);
					_subLayer[i+1]->setPreferredSize(subLayerSize);
					_subLayer[i+1]->needsLayout();
				}
				i = 2;
				layerPosition = layerRect.leftTop();
				layerPosition.x += sideFrameWidth;
				layerPosition.y -= subLayerHeight;
				layerPosition.y -= subLayerHeight;

				_subLayer[i]->setPosition(layerPosition);
				_subLayer[i]->setPreferredSize(CCSizeMake(subLayerSize.width * 2, subLayerSize.height));
				_subLayer[i]->needsLayout();
						}
						break;
			case(4):	{
				for(SLSize i(0); i < 4; i=i+2)	{

					if(i == 2)	{
						layerPosition = layerRect.leftTop();
						layerPosition.x += sideFrameWidth;
						layerPosition.y -= subLayerHeight + NL_MAINLAYER_FRAME_HEIGHT;
						layerPosition.y -= subLayerHeight;
					}

					layerPosition.y += NL_MAINLAYER_FRAME_HEIGHT;

					_subLayer[i]->setPosition(layerPosition);
					_subLayer[i]->setPreferredSize(subLayerSize);
					_subLayer[i]->needsLayout();

					layerPosition.x += subLayerWidth;
					_subLayer[i+1]->setPosition(layerPosition);
					_subLayer[i+1]->setPreferredSize(subLayerSize);
					_subLayer[i+1]->needsLayout();
				}
						}
						break;
			}
		}
	}

	void CCControlQuadLayer::setContentSize(const CCSize& contentSize)	{
		SLBaseClass::setContentSize(contentSize);
	}

	void CCControlQuadLayer::recreate(SLSize numSublayers)	{
		for(SLSize i(0); i < 4; ++i)	{
			if(_subLayer[i] != nullptr)	{
				_subLayer[i]->removeFromParent();
				_subLayer[i] = nullptr;
			}
		}
		setNumSubLayers(numSublayers);
		addSubLayerContent();
	}

	void CCControlQuadLayer::addSubLayerContent()
	{
		CCControlBase* layer(this);
		// create maximum 4 layers
		//
		// 0 | 1
		// 2 | 3

		const SLSize numSubLayers = sl::min(SLSize(4), getNumSubLayers());

		const CCSize& contentSize(layer->getContentSize());

		// determine the size for a single sublayer
		const float sideFrameWidth(NL_SUBLAYER_FRAME_WIDTH);
		const float subLayerWidth((contentSize.width - sideFrameWidth * 2.0f) * 0.5f);
		const float subLayerHeight((contentSize.height - NL_MAINLAYER_FRAME_HEIGHT) * 0.5f);

		CCSize subLayerSize(CCSizeMake(subLayerWidth, subLayerHeight));

		for(SLSize i(0); i < numSubLayers; ++i)	{
			_subLayer[i] = ControlUtils::createTwinControl(subLayerSize, getLeftLayerClassName(), getRightLayerClassName());
		}

		// layout
		NodeRect layerRect(NodeRect(layer,true));
		CCPoint layerPosition(layerRect.leftTop());
		layerPosition.x += NL_MAINLAYER_FRAME_WIDTH;
		layerPosition.y -= subLayerHeight + NL_MAINLAYER_FRAME_HEIGHT;

		switch(numSubLayers)	{
		case(1):	{
			SLSize i(0);
			layerPosition.y += NL_MAINLAYER_FRAME_WIDTH;
			layerPosition.y -= subLayerSize.height;
			_subLayer[i]->setPosition(layerPosition);

			_subLayer[i]->setPreferredSize(CCSizeMake(subLayerSize.width * 2, subLayerSize.height * 2));

			layer->addChild(_subLayer[i],0);
					}
					break;
		case(2):	{
			for(SLSize i(0); i < 2; i = i + 2)	{

				layerPosition.y += NL_MAINLAYER_FRAME_WIDTH;
				layerPosition.y -= subLayerSize.height;

				_subLayer[i]->setPosition(layerPosition);
				layerPosition.x += subLayerWidth;
				_subLayer[i+1]->setPosition(layerPosition);

				_subLayer[i]->setPreferredSize(CCSizeMake(subLayerSize.width, subLayerSize.height * 2));
				_subLayer[i+1]->setPreferredSize(CCSizeMake(subLayerSize.width, subLayerSize.height * 2));

				layer->addChild(_subLayer[i],0);
				layer->addChild(_subLayer[i+1],0);
			}
					}
					break;
		case(3):	{
			SLSize i(0);
			for(i; i < 2; i = i + 2)	{
				layerPosition.y += NL_MAINLAYER_FRAME_WIDTH;

				_subLayer[i]->setPosition(layerPosition);
				layerPosition.x += subLayerWidth;
				_subLayer[i+1]->setPosition(layerPosition);

				layer->addChild(_subLayer[i],0);
				layer->addChild(_subLayer[i+1],0);
			}
			i = 2;
			layerPosition = layerRect.leftTop();
			layerPosition.x += sideFrameWidth;
			layerPosition.y -= subLayerHeight;
			layerPosition.y -= subLayerHeight;

			layerPosition.y += NL_MAINLAYER_FRAME_WIDTH;

			_subLayer[i]->setPosition(layerPosition);
			_subLayer[i]->setPreferredSize(CCSizeMake(subLayerSize.width * 2, subLayerSize.height));

			layer->addChild(_subLayer[i],0);
					}
					break;
		case(4):	{
			for(SLSize i(0); i < 4; i = i + 2)	{
				if(i == 2)	{
					layerPosition = layerRect.leftTop();
					layerPosition.x += NL_MAINLAYER_FRAME_WIDTH;
					layerPosition.y -= subLayerHeight + NL_MAINLAYER_FRAME_HEIGHT;
					layerPosition.y -= subLayerHeight;
				}

				layerPosition.y += NL_MAINLAYER_FRAME_WIDTH;

				_subLayer[i]->setPosition(layerPosition);
				layerPosition.x += subLayerWidth;
				_subLayer[i+1]->setPosition(layerPosition);

				layer->addChild(_subLayer[i],0);
				layer->addChild(_subLayer[i+1],0);
			}
					}
					break;
		}

	}

}


