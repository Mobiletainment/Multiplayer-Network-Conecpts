//!---------------------------------------------------------------------------
//! \file "CCControlRow.h"
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
#ifndef __CCCONTROL_ROW_H__
#define __CCCONTROL_ROW_H__

#include "CCControlContainer.h"

namespace sl	{
	class slCocosUtil_API CCControlRow : public CCControlContainer
	{
		SL_DECLARE_BASE(CCControlContainer)
	public:

		SL_IMPLEMENT_CLASSNAME(CCControlRow)

		static CCControlRow* controlRowFromNodes(
			CCScale9Sprite* backgroundSprite,
			CCArray* nodes,
			const CCSize& preferredSize
			);

		virtual void needsLayout();

	protected:
		CCControlRow();
		~CCControlRow();

	private:
	};
}

#endif
