//!---------------------------------------------------------------------------
//! \file "CCControlColumn.cpp"
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
#include "CCControlColumn.h"
#include "../slValueConversion.h"
#include "../slControlUtils.h"

namespace sl	{
	CCControlColumn::CCControlColumn()
	{
		getCtrlLayoutFlags().addFlag(ECtrlLayoutFlag_TopDown);
	}

	CCControlColumn::~CCControlColumn()
	{
	}

	CCControlColumn* CCControlColumn::controlColumnFromNodes(
		CCScale9Sprite* backgroundSprite,
		CCArray* nodes,
		const CCSize& preferredSize
		)
	{
		CCControlColumn *ctrl(new CCControlColumn());
		if(ctrl->initFromNodes(backgroundSprite, nodes, preferredSize))	{
			ctrl->autorelease();
			return ctrl;
		}
		CC_SAFE_RELEASE_NULL(ctrl);
		return ctrl;
	}

	void CCControlColumn::needsLayout()
	{
		if (getBackGroundSprite() == nullptr) {
			return;
		}


		// iterate once to get the full height for this container
		CCArray* nodes(getNodes());
		if(nodes != nullptr)	{

			// first store the current preferred size
			CCSize currentPreferredSize(getPreferredSize());

#if 0
			// temporary set a new contentsize
			// so children will use this to layout themselfes
			CCSize currentContentSize(getContentSize());
			setContentSize(CCSizeMake(currentPreferredSize.width, currentContentSize.height));
#endif
			float paddingY = static_cast<float>(m_marginH);
			float targetHeight = static_cast<float>(m_marginV);

			CCObject* object;
			if(nodes->count() > 0)	{
				CCARRAY_FOREACH(nodes, object)
				{
					CCNode* node(dynamic_cast<CCNode*>(object));
					if(node != nullptr)	{
						CCControl* subCtrl(dynamic_cast<CCControl*>(object));
						if(subCtrl != nullptr)	{
							CCControlBase* subCtrlBase(dynamic_cast<CCControlBase*>(object));
							if(subCtrlBase != nullptr)	{
							}
							else	{

							}
							subCtrl->needsLayout();
						}
						const CCSize& nodeSize(node->getContentSize()); 
						targetHeight += nodeSize.height;
						targetHeight += paddingY;
					}
				}
				targetHeight -= paddingY;
			}
			targetHeight += m_marginV;

			float nodeMaxWidth(20);

			CCPoint ctrlPosition(CCPointMake(m_marginH, targetHeight - m_marginV));
			CCARRAY_FOREACH(nodes, object)
			{
				CCNode* node(dynamic_cast<CCNode*>(object));
				if(node != nullptr)	{
					const CCSize& nodeSize(node->getContentSize()); 
					nodeMaxWidth = std::max(nodeSize.width, nodeMaxWidth);

					ctrlPosition.y -= nodeSize.height;
					node->setPosition(ctrlPosition);
					ctrlPosition.y -= paddingY;
				}
			}

			float width(nodeMaxWidth + m_marginH * 2);
			float height(targetHeight);

			if(currentPreferredSize.width != 0.0f)	{
				width = currentPreferredSize.width;
			}
			else	{
				width = sl::max(currentPreferredSize.width, width);
			}

			if(currentPreferredSize.height != 0.0f)	{
				height = currentPreferredSize.height;
			}
			else	{
				height = sl::max(currentPreferredSize.height, height);
			}

			setPreferredSize(CCSizeMake(width, height));
		}

		SLBaseClass::needsLayout();
	}


}


