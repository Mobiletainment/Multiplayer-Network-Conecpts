//!---------------------------------------------------------------------------
//! \file "ControlFactory.cpp"
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
#include "ControlFactory.h"

#include "CCControlCheckBox.h"
#include "CCControlColumn.h"
#include "CCControlContainer.h"
#include "CCControlEditBox.h"
#include "CCControlQuadLayer.h"
#include "CCControlRow.h"
#include "CCControlTwinLayer.h"
#include "CCControlValueBox.h"
#include "../slControlUtils.h"


namespace sl	{

	typedef std::hash_map<SLAString, slFunc_createControl*> TCreateControlLookupMap;

	TCreateControlLookupMap _createControlLookupMap;

	CCControlBase* ControlFactory::createControl( 
		const char* className,
		const CCSize& preferredSize,
		CCScale9Sprite* backgroundSprite)
	{
		if(sl::isEmptyString(className))	{
			SL_ASSERT(false);
			className = CCControlBase::staticClassName();
		}
		CCControlBase* createdControl(nullptr);
		SLAString createClassName(className);

		if(createClassName.compare(CCControlBase::staticClassName()) == 0)	{
			createdControl = ControlUtils::createBaseControl(preferredSize, backgroundSprite);
		}
		else if(createClassName.compare(CCControlCheckBox::staticClassName()) == 0)	{

		}
		else if(createClassName.compare(CCControlColumn::staticClassName()) == 0)	{
			createdControl = ControlUtils::createControlColumn(nullptr, preferredSize);
		}
		else if(createClassName.compare(CCControlEditBox::staticClassName()) == 0)	{

		}
		else if(createClassName.compare(CCControlQuadLayer::staticClassName()) == 0)	{

		}
		else if(createClassName.compare(CCControlTwinLayer::staticClassName()) == 0)	{

		}
		else if(createClassName.compare(CCControlRow::staticClassName()) == 0)	{
			createdControl = ControlUtils::createControlRow(nullptr, preferredSize);
		}
		else if(createClassName.compare(CCControlValueBox::staticClassName()) == 0)	{

		}

		// try the lookup map
		if(createdControl == nullptr)	{
			TCreateControlLookupMap::const_iterator i(_createControlLookupMap.find(className));
			if(i != _createControlLookupMap.end())
			{
				slFunc_createControl* functionCreateControl((*i).second);
				if(functionCreateControl != nullptr)	{
					createdControl = functionCreateControl(className, preferredSize, backgroundSprite);
				}
			}
		}

		if(createdControl == nullptr)	{
			createdControl = ControlUtils::createBaseControl(preferredSize);
		}
		return createdControl;
	}

	void ControlFactory::removeCreateControlCallbacks()
	{
		_createControlLookupMap.clear();
	}

	void ControlFactory::addCreateControlCallBack( const char* className, slFunc_createControl* function )
	{
		_createControlLookupMap[className] = function;
	}

}


