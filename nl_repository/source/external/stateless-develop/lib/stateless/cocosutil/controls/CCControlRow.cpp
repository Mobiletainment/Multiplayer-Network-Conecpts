//!---------------------------------------------------------------------------
//! \file "CCControlRow.cpp"
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
#include "CCControlRow.h"
#include "../slValueConversion.h"
#include "../slControlUtils.h"

namespace sl	{
	CCControlRow::CCControlRow()
	{
		getCtrlLayoutFlags().addFlag(ECtrlLayoutFlag_LeftRight);
	}

	CCControlRow::~CCControlRow()
	{
	}

	CCControlRow* CCControlRow::controlRowFromNodes(
		CCScale9Sprite* backgroundSprite,
		CCArray* nodes,
		const CCSize& preferredSize
		)
	{
		CCControlRow *ctrl(new CCControlRow());
		if(ctrl->initFromNodes(backgroundSprite, nodes, preferredSize))	{
			ctrl->autorelease();
			return ctrl;
		}
		CC_SAFE_RELEASE_NULL(ctrl);
		return ctrl;
	}

	void CCControlRow::needsLayout()
	{
		if (getBackGroundSprite() == nullptr) {
			return;
		}

		float nodeMaxHeight(20);
		float height=20;
		float width(100);
		CCArray* nodes(getNodes());
		if(nodes != nullptr)	{

			if(nodes->count() > 0)	{
				width = static_cast<float>(m_marginH);
				float padding = 10;

				CCObject* object;


				CCARRAY_FOREACH(nodes, object)
				{
					CCNode* node(dynamic_cast<CCNode*>(object));
					if(node != nullptr)	{
						CCControl* subCtrl(dynamic_cast<CCControl*>(object));
						if(subCtrl != nullptr)	{
							subCtrl->needsLayout();
						}
						const CCSize& nodeSize(node->getContentSize()); 
						nodeMaxHeight = std::max(nodeSize.height, nodeMaxHeight);
					}
				}

				height = m_marginV * 2.0f + nodeMaxHeight;

				CCPoint ctrlPosition(CCPointMake(m_marginH, m_marginV));
				CCARRAY_FOREACH(nodes, object)
				{
					CCNode* node(dynamic_cast<CCNode*>(object));
					if(node != nullptr)	{

						// center the control vertically
						const CCSize& nodeSize(node->getContentSize()); 
						ctrlPosition.y = height * 0.5f - nodeSize.height * 0.5f;

						node->setPosition(ctrlPosition);
						width += nodeSize.width + padding;
						ctrlPosition.x += nodeSize.width + padding;
					}
				}

				width -= padding;
				width += m_marginH;
			}

		}



		CCSize preferredSize(getPreferredSize());
		if(width != preferredSize.width && preferredSize.width != 0)	{
			width = preferredSize.width;
		}
		setPreferredSize(CCSizeMake(width, height));

		SLBaseClass::needsLayout();
	}

}


