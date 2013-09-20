//!---------------------------------------------------------------------------
//! \file "slControlUtils.h"
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
#ifndef __SLCONTROLUTILS_H__
#define __SLCONTROLUTILS_H__

#include "slCocosUtilMacros.h"
#include "controls/CCControlBase.h"
#include "controls/CCControlTwinLayer.h"
#include "controls/CCControlCheckBox.h"
#include "controls/CCControlEditBox.h"
#include "controls/CCControlQuadLayer.h"
#include "controls/CCControlValueBox.h"
#include "controls/CCControlRow.h"
#include "controls/CCControlColumn.h"

#include "ui/slDialog.h"

namespace sl	{
	class slCocosUtil_API ControlUtils
	{
	public:
		static CCControlColumn* createControlColumn(
			CCArray* nodes, const CCSize& preferredSize);

		static CCControlRow* createControlRow(
			CCArray* nodes, const CCSize& preferredSize);

		static CCControlButton* createButton(
			const char * title);

		static CCControlCheckBox* createCheckBox(
			const char* checkedIconSpriteName,
			const char* uncheckedIconSpriteName,
			CCScale9Sprite* backgroundSprite,
			const char* title, const CCSize& preferredSize);

		static CCControlCheckBox* createCheckBox(
			const char* title, const CCSize& preferredSize);

		static CCNode* createLabel( 
			const char* title, 
			CCTextAlignment alignment = kCCTextAlignmentCenter );

		static CCControlEditBox* createEditBox(
			const char* title,
			const CCSize& preferredSize);

		static CCScale9Sprite* createCtrlBackground();

		static CCScale9Sprite* createSubViewBackground();

		static CCScale9Sprite* createMainViewBackground();

		static CCScale9Sprite* createOpaqueCtrlBackground();

		static CCControlValueBox* createValueBox(
			const char* title,
			const CCSize& preferredSize,
			const char* text = nullptr);


		static CCControlBase* createBaseControl(
			const CCSize& ctrlSize, CCScale9Sprite* backgroundSprite = nullptr
			);

		static CCControlTwinLayer* createTwinControl(
			const CCSize& ctrlSize,
			CCString* leftLayerClassName = nullptr, CCString* rightLayerClassName = nullptr
			);

		static CCControlQuadLayer* createQuadControl(
			const CCSize& ctrlSize, SLSize numSublayers = 4,
			CCString* leftLayerClassName = nullptr, CCString* rightLayerClassName = nullptr
			);

		static void addCtrlNode(CCNode* node, CCNode* toLayer);

	};

}


#endif // __SLPHYSICSUTILS_H__
