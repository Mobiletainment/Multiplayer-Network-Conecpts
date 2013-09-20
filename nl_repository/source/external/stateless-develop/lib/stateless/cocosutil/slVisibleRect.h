//!---------------------------------------------------------------------------
//! \file "slVisibleRect.h"
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
#ifndef __SLVISIBLERECT_H__
#define __SLVISIBLERECT_H__

namespace sl	{

	class slCocosUtil_API NodeRect	{
	public:
		NodeRect(CCNode* node, bool localSpace);

		void invalidate();

		CCRect getNodeRect();

		CCPoint left();
		CCPoint right();
		CCPoint top();
		CCPoint bottom();
		CCPoint center();
		CCPoint leftTop();
		CCPoint rightTop();
		CCPoint leftBottom();
		CCPoint rightBottom();

		static CCRect getRect(CCNode * node);

	private:
		void lazyInit();
		CCRect _visibleRect;
		CCNode* _node;
		bool _localSpace;
	};

	class slCocosUtil_API VisibleRect
	{
	public:
		static void invalidate();

		static const CCRect& getVisibleRect();

		static CCPoint left();
		static CCPoint right();
		static CCPoint top();
		static CCPoint bottom();
		static CCPoint center();
		static CCPoint leftTop();
		static CCPoint rightTop();
		static CCPoint leftBottom();
		static CCPoint rightBottom();
	private:
		static void lazyInit();
		static CCRect s_visibleRect;
	};
}

#endif /* __VISIBLERECT_H__ */
