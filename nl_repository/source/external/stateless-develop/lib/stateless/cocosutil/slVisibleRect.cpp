//!---------------------------------------------------------------------------
//! \file "slVisibleRect.cpp"
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
#include "slVisibleRect.h"

namespace sl	{

	NodeRect::NodeRect( CCNode* node, bool localSpace )
		:_node(node)
		,_localSpace(localSpace)
	{
		invalidate();
		lazyInit();
	}

	void NodeRect::invalidate()
	{
		_visibleRect.size.width = 0.0f;
		_visibleRect.size.height = 0.0f;
	}

	void NodeRect::lazyInit()
	{
		if (_visibleRect.size.width == 0.0f && _visibleRect.size.height == 0.0f)
		{
			if(_node != NULL)	{
				if(_localSpace)	{
					_visibleRect.origin = ccp(0.0f,0.0f);
				}
				else	{
					_visibleRect.origin = _node->getPosition();
				}
				_visibleRect.size = _node->getContentSize();
			}
		}
	}

	CCRect NodeRect::getRect(CCNode * pNode)
	{
		CCRect rc;
		rc.origin = pNode->getPosition();
		rc.size = pNode->getContentSize();
		rc.origin.x -= rc.size.width / 2;
		rc.origin.y -= rc.size.height / 2;
		return rc;
	}

	CCRect NodeRect::getNodeRect()
	{
		lazyInit();
		return CCRectMake(_visibleRect.origin.x, _visibleRect.origin.y, _visibleRect.size.width, _visibleRect.size.height);
	}

	CCPoint NodeRect::left()
	{
		lazyInit();
		return ccp(_visibleRect.origin.x, _visibleRect.origin.y+_visibleRect.size.height/2);
	}

	CCPoint NodeRect::right()
	{
		lazyInit();
		return ccp(_visibleRect.origin.x+_visibleRect.size.width, _visibleRect.origin.y+_visibleRect.size.height/2);
	}

	CCPoint NodeRect::top()
	{
		lazyInit();
		return ccp(_visibleRect.origin.x+_visibleRect.size.width/2, _visibleRect.origin.y+_visibleRect.size.height);
	}

	CCPoint NodeRect::bottom()
	{
		lazyInit();
		return ccp(_visibleRect.origin.x+_visibleRect.size.width/2, _visibleRect.origin.y);
	}

	CCPoint NodeRect::center()
	{
		lazyInit();
		return ccp(_visibleRect.origin.x+_visibleRect.size.width/2, _visibleRect.origin.y+_visibleRect.size.height/2);
	}

	CCPoint NodeRect::leftTop()
	{
		lazyInit();
		return ccp(_visibleRect.origin.x, _visibleRect.origin.y+_visibleRect.size.height);
	}

	CCPoint NodeRect::rightTop()
	{
		lazyInit();
		return ccp(_visibleRect.origin.x+_visibleRect.size.width, _visibleRect.origin.y+_visibleRect.size.height);
	}

	CCPoint NodeRect::leftBottom()
	{
		lazyInit();
		return _visibleRect.origin;
	}

	CCPoint NodeRect::rightBottom()
	{
		lazyInit();
		return ccp(_visibleRect.origin.x+_visibleRect.size.width, _visibleRect.origin.y);
	}







	CCRect VisibleRect::s_visibleRect;


	void VisibleRect::invalidate()
	{
		s_visibleRect.size.width = 0.0f;
		s_visibleRect.size.height = 0.0f;
	}


	void VisibleRect::lazyInit()
	{
		if (s_visibleRect.size.width == 0.0f && s_visibleRect.size.height == 0.0f)
		{
			CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
			s_visibleRect.origin = pEGLView->getVisibleOrigin();
			s_visibleRect.size = pEGLView->getVisibleSize();
		}
	}

	const CCRect& VisibleRect::getVisibleRect()
	{
		lazyInit();
		return s_visibleRect;
//		return CCRectMake(s_visibleRect.origin.x, s_visibleRect.origin.y, s_visibleRect.size.width, s_visibleRect.size.height);
	}

	CCPoint VisibleRect::left()
	{
		lazyInit();
		return ccp(s_visibleRect.origin.x, s_visibleRect.origin.y+s_visibleRect.size.height/2);
	}

	CCPoint VisibleRect::right()
	{
		lazyInit();
		return ccp(s_visibleRect.origin.x+s_visibleRect.size.width, s_visibleRect.origin.y+s_visibleRect.size.height/2);
	}

	CCPoint VisibleRect::top()
	{
		lazyInit();
		return ccp(s_visibleRect.origin.x+s_visibleRect.size.width/2, s_visibleRect.origin.y+s_visibleRect.size.height);
	}

	CCPoint VisibleRect::bottom()
	{
		lazyInit();
		return ccp(s_visibleRect.origin.x+s_visibleRect.size.width/2, s_visibleRect.origin.y);
	}

	CCPoint VisibleRect::center()
	{
		lazyInit();
		return ccp(s_visibleRect.origin.x+s_visibleRect.size.width/2, s_visibleRect.origin.y+s_visibleRect.size.height/2);
	}

	CCPoint VisibleRect::leftTop()
	{
		lazyInit();
		return ccp(s_visibleRect.origin.x, s_visibleRect.origin.y+s_visibleRect.size.height);
	}

	CCPoint VisibleRect::rightTop()
	{
		lazyInit();
		return ccp(s_visibleRect.origin.x+s_visibleRect.size.width, s_visibleRect.origin.y+s_visibleRect.size.height);
	}

	CCPoint VisibleRect::leftBottom()
	{
		lazyInit();
		return s_visibleRect.origin;
	}

	CCPoint VisibleRect::rightBottom()
	{
		lazyInit();
		return ccp(s_visibleRect.origin.x+s_visibleRect.size.width, s_visibleRect.origin.y);
	}
}
