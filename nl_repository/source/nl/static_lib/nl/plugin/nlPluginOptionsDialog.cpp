//!---------------------------------------------------------------------------
//! \file "nlPluginOptionsDialog.cpp"
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

#include "nlPluginOptionsDialog.h"
#include "nlPluginLayerContent.h"

namespace nl	{

	PluginLayerContent* PluginOptionsDialog::_pluginLayerContent(nullptr);

	PluginOptionsDialog::PluginOptionsDialog()
		// TODO: @student : check this code - this is an example how to track edit box changes
		:_notificationPhysicsFrameRateChanged(SL_NOTIFY_CTRL_EDIT_END)
		,_ctrlPhysicsFrameRate(nullptr)
	{
	}

	PluginOptionsDialog::~PluginOptionsDialog()	{
		setPluginLayerContent(nullptr);
	}

	bool PluginOptionsDialog::init()	{
		CCDictionary* dataSource(NetworkState::getInstanceDictionary(
			SL_SERIALIZEKEY_PLUGIN_GLOBAL_SETTINGS));

		// configure once
		Dictionary::setObjectIfNotExist(dataSource, 
			CCString::create("networklessons"),SL_SERIALIZEKEY_PEER_SESSION_PASSWORD);
		Dictionary::setObjectIfNotExist(dataSource, 
			CCDouble::create(50),SL_SERIALIZEKEY_PHYSICSFRAMERATE);

		setDataSource(dataSource);

		CCSize ctrlSize(CCSizeMake(0,0));

		CCArray* optionCtrls = CCArray::create();

		CCControlBase* ctrlContainer(nullptr);
		if(_pluginLayerContent != nullptr)	{
			if(_pluginLayerContent->getPluginHasNetwork())	{
				ctrlContainer = ControlUtils::createEditBox("Session password:", ctrlSize);
				ctrlContainer->setValueKey(SL_SERIALIZEKEY_PEER_SESSION_PASSWORD);
				ctrlContainer->getCtrlLayoutFlags().addFlag(ECtrlLayoutFlag_ResetPreferredSizeOnSerialize);
				ctrlContainer->deserialize(dataSource);
				optionCtrls->addObject(ctrlContainer);
			}
			if(_pluginLayerContent->getPluginHasPhysics())	{
				ctrlContainer = ControlUtils::createEditBox("Physics framerate (hz):", ctrlSize);
				ctrlContainer->setValueKey(SL_SERIALIZEKEY_PHYSICSFRAMERATE);
				ctrlContainer->getCtrlLayoutFlags().addFlag(ECtrlLayoutFlag_ResetPreferredSizeOnSerialize);
				ctrlContainer->deserialize(dataSource);
				optionCtrls->addObject(ctrlContainer);
				// TODO: @student : check this code - this is an example how to track edit box changes
				_ctrlPhysicsFrameRate = dynamic_cast<CCControlEditBox*>(ctrlContainer);
			}
		}

		if(optionCtrls->count() == 0)	{
			ctrlContainer = ControlUtils::createValueBox("Option Info:", ctrlSize, "no plugin wide options available");
			optionCtrls->addObject(ctrlContainer);
		}

		CCControlColumn* ctrlOptionsContainer = ControlUtils::createControlColumn(optionCtrls,ctrlSize);
		ctrlOptionsContainer->getCtrlFlags().removeFlag(ECtrlFlag_UseScissorTest);

		CCArray* dialogCtrls = CCArray::create();
		dialogCtrls->addObject(ctrlOptionsContainer);
		dialogCtrls->addObject(createOkBtn());

		CCControlColumn* ctrlDialogContainer = ControlUtils::createControlColumn(dialogCtrls,ctrlSize);
		ctrlDialogContainer->getCtrlFlags().removeFlag(ECtrlFlag_UseScissorTest);

		CCSize dlgSize(ctrlDialogContainer->getContentSize());
		setPreferredSize(ctrlSize);

		// call the base class here we don't want all those background sprites
		bool initialized(CCControlBase::init());

		addChild(ctrlDialogContainer);
		setCtrlColumn(ctrlDialogContainer);

		// read the data from the datastore
		// we need to redo the layout as we have some edit controls 
		// inside this dialog
		// so force a new layout
		deserializeAll(getDataSource(), true);

		return initialized;
	}

	void PluginOptionsDialog::onEnter()	{
		SLBaseClass::onEnter();
		// TODO: @student : check this code - this is an example how to track edit box changes
		_notificationPhysicsFrameRateChanged.addObserver(
			this, callfuncO_selector(PluginOptionsDialog::onPhysicsFrameRateChangedNotification));
	}

	void PluginOptionsDialog::onExit()	{
		// TODO: @student : check this code - this is an example how to track edit box changes
		_notificationPhysicsFrameRateChanged.removeObserver();
		SLBaseClass::onExit();
	}

	void PluginOptionsDialog::onUIAction( CCObject *sender, CCControlEvent controlEvent )
	{
		SLBaseClass::onUIAction(sender, controlEvent);
#ifdef _DEBUG
		const int tag(Dialog::getTag(sender));
		switch(tag)	{
		case SL_CTRLID_OK:	{
			NetworkState* networkState(NetworkState::sharedInstance());
			networkState->save();
							}
							break;
		}
#endif
	}

	// TODO: @student : check this code - this is an example how to track edit box changes
	void PluginOptionsDialog::onPhysicsFrameRateChangedNotification(CCObject* sender)	{
		// note: the notification center spreads the function call
		//       to every registered observer
		//       so you have to explicitly check if 
		//       the sender is the object you want to deal with
		CCControlEditBox* editBox(dynamic_cast<CCControlEditBox*>(sender));
		if(editBox == _ctrlPhysicsFrameRate)	{
			// the value should have been serialized to the dataSource at this time 
			// so now we can check the dictionary value
			CCDictionary* dataSource(NetworkState::getInstanceDictionary(
				SL_SERIALIZEKEY_PLUGIN_GLOBAL_SETTINGS));
			double physicsFrameRate(Dictionary::getDouble(
				dataSource, SL_SERIALIZEKEY_PHYSICSFRAMERATE, 50.0));
			// and pass the value to the physics config
			PhysicsConfig::sharedInstance().setFrequency(physicsFrameRate);
		}
	}
}

