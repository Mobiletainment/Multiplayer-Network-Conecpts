//!---------------------------------------------------------------------------
//! \file "nlPluginTasksDialog.cpp"
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

#include "nlPluginTasksDialog.h"

namespace nl	{
	PluginTasksDialog::PluginTasksDialog()
	{

	}

	PluginTasksDialog::~PluginTasksDialog()
	{

	}


	bool PluginTasksDialog::init()
	{
		CCDictionary* dataSource(NetworkState::sharedInstance()->getSubDictionary(SL_SERIALIZEKEY_PLUGIN_GLOBAL_SETTINGS));

		NetworkState* networkState(NetworkState::sharedInstance());
		Dictionary::setObjectIfNotExist(dataSource, 
			"info\ninfo\ninfo\ninfo"
			,SL_SERIALIZEKEY_PLUGIN_TASKS);
		Dictionary::setObjectIfNotExist(dataSource, 
			"- search for TODO: @student to find some entry points\n"
			"- read the documentation\nread the code\ncheck with google\nthink"
			,SL_SERIALIZEKEY_PLUGIN_TASK_HINTS);

		setDataSource(dataSource);

		CCSize preferredSize(CCSizeMake(0,0));

		CCControlColumn* ctrlInfoContainer = ControlUtils::createControlColumn(nullptr,preferredSize);

		nl::CCControlBase* ctrlValueBox;
		ctrlValueBox = nl::ControlUtils::createValueBox("Tasks:", preferredSize);
		ctrlValueBox->getCtrlLayoutFlags().addFlag(ECtrlLayoutFlag_ResetPreferredSizeOnSerialize);
		ctrlValueBox->setValueKey(SL_SERIALIZEKEY_PLUGIN_TASKS);
		ctrlValueBox->deserialize(dataSource);
		ctrlInfoContainer->addNode(ctrlValueBox);

		ctrlValueBox = nl::ControlUtils::createValueBox("Hints:", preferredSize);
		ctrlValueBox->getCtrlLayoutFlags().addFlag(ECtrlLayoutFlag_ResetPreferredSizeOnSerialize);
		ctrlValueBox->setValueKey(SL_SERIALIZEKEY_PLUGIN_TASK_HINTS);
		ctrlValueBox->deserialize(dataSource);
		ctrlInfoContainer->addNode(ctrlValueBox);

		CCControlColumn* ctrlDialogContainer = ControlUtils::createControlColumn(nullptr,preferredSize);
		ctrlDialogContainer->addNode(ctrlInfoContainer);
		ctrlDialogContainer->addNode(createOkBtn());

		CCSize dlgSize(ctrlDialogContainer->getContentSize());

		// call the base class here we don't want all those background sprites
		CCControlBase::init();

		addChild(ctrlDialogContainer);

		// read the data from the datastore
		// deserializeAll(getDataSource());

		ctrlInfoContainer->setPreferredSize(preferredSize);
		ctrlInfoContainer->needsLayout();

		ctrlDialogContainer->setPreferredSize(preferredSize);
		ctrlDialogContainer->needsLayout();

		dlgSize = ctrlDialogContainer->getContentSize();
		setPreferredSize(dlgSize);

		needsLayout();

		return true;
	}

	void PluginTasksDialog::onUIAction( CCObject *sender, CCControlEvent controlEvent )
	{
		SLBaseClass::onUIAction(sender, controlEvent);
#if 0 //def _DEBUG
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

}

