//!---------------------------------------------------------------------------
//! \file "CCControlContainer.cpp"
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
#include "CCControlContainer.h"

namespace sl	{

	CCControlContainer::CCControlContainer()
		:_nodes(nullptr)
	{
		getCtrlFlags().addFlag(ECtrlFlag_UseScissorTest);
	}

	CCControlContainer::~CCControlContainer()	{
		setNodes(nullptr);
	}

	void CCControlContainer::addNodeAt(CCNode* node, unsigned int idx)	{
		if(getNodes() == nullptr)	{
			addNode(node);
		}
		else	{
			unsigned int nodeCount(_nodes->count());
			if(nodeCount > 1)	{
				idx = sl::min(nodeCount - 1, idx);
			}
			_nodes->insertObject(node, idx);
			addChild(node, 1);
			node->setAnchorPoint(ccp(0,0.0f)); 
			needsLayout();
		}
	}

	void CCControlContainer::addNode(CCNode* node)	{
		CCArray* uiNodes(CCArray::create());
		uiNodes->addObject(node);
		addNodes(uiNodes);
	}

	void CCControlContainer::addNodes( CCArray* nodes )
	{
		if(getNodes() == nullptr)	{
			setNodes(nodes);
		}
		else	{
			CCObject* object;
			CCARRAY_FOREACH(nodes, object)
			{
				CCNode* node(dynamic_cast<CCNode*>(object));
				if(node != nullptr)	{
					_nodes->addObject(node);
				}
			}
		}

		if(nodes != nullptr)	{
			CCObject* object;
			CCARRAY_FOREACH(nodes, object)
			{
				CCNode* node(dynamic_cast<CCNode*>(object));
				if(node != nullptr)	{
					addChild(node, 1);
					node->setAnchorPoint(ccp(0,0.0f)); 
				}
			}

			needsLayout();
		}
	}

	bool CCControlContainer::initFromNodes(
		CCScale9Sprite* backgroundSprite,
		CCArray* nodes,
		const CCSize& preferredSize
		)
	{
		//		setCascadeOpacityEnabled(true);
		//		setCascadeColorEnabled(true);


		if (!SLBaseClass::initWithBackGroundSprite(backgroundSprite))
			return false;

		setPreferredSize(preferredSize);

		addNodes(nodes);
		if(nodes == nullptr)	{
			needsLayout();
		}

		return true;
	}

	bool CCControlContainer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
	{
		if (!isTouchInside(pTouch) || !isEnabled())
		{
			return false;
		}

		sendActionsForControlEvents(CCControlEventTouchDown);
		return true;
	}

	void CCControlContainer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
	{    
		if (isTouchInside(pTouch))	{
		}	
	}

	void CCControlContainer::serialize( CCDictionary* target )
	{
		// serialize sub nodes
	}

	void CCControlContainer::deserialize( CCDictionary* source )
	{
		// deserialize sub nodes
	}

	void CCControlContainer::redoLayout()
	{
		// as we need to redo the layout now
		// we need to reset the preferredHeight
		// TODO support Max and Min size
		if(getCtrlLayoutFlags().hasFlag(ECtrlLayoutFlag_TopDown))	{
			CCSize ctrlNewPrefferedSize(getPreferredSize());
			ctrlNewPrefferedSize.height = 0;
			setPreferredSize(ctrlNewPrefferedSize);
			needsLayout();
		}

		if(getCtrlLayoutFlags().hasFlag(ECtrlLayoutFlag_LeftRight))	{
			CCSize ctrlNewPrefferedSize(getPreferredSize());
			ctrlNewPrefferedSize.width = 0;
			setPreferredSize(ctrlNewPrefferedSize);
			needsLayout();
		}
	}



}


