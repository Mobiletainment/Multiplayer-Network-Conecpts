//!---------------------------------------------------------------------------
//! \file "nlPluginInfoDialog.cpp"
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

#include "nlPluginInfoDialog.h"

namespace nl	{
	PluginInfoDialog::PluginInfoDialog()
	{

	}

	PluginInfoDialog::~PluginInfoDialog()
	{

	}

	bool PluginInfoDialog::init()
	{
		CCDictionary* dataSource(NetworkState::sharedInstance()->getSubDictionary(SL_SERIALIZEKEY_PLUGIN_GLOBAL_SETTINGS));

		// configure once
		Dictionary::setObjectIfNotExist(dataSource,"missing brief info", SL_SERIALIZEKEY_PLUGIN_INFO_BRIEF);
		Dictionary::setObjectIfNotExist(dataSource,"missing detail info", SL_SERIALIZEKEY_PLUGIN_INFO_DETAIL);

		setDataSource(dataSource);

		CCSize preferredSize(CCSizeMake(0,0));

		CCControlColumn* ctrlInfoContainer = ControlUtils::createControlColumn(nullptr,preferredSize);

		CCControlBase* ctrlValueBox;
		ctrlValueBox = ControlUtils::createValueBox("Brief:", preferredSize);
		ctrlValueBox->getCtrlLayoutFlags().addFlag(ECtrlLayoutFlag_ResetPreferredSizeOnSerialize);
		ctrlValueBox->setValueKey(SL_SERIALIZEKEY_PLUGIN_INFO_BRIEF);
		ctrlValueBox->deserialize(dataSource);
		ctrlInfoContainer->addNode(ctrlValueBox);

		ctrlValueBox = ControlUtils::createValueBox("Details:", preferredSize);
		ctrlValueBox->getCtrlLayoutFlags().addFlag(ECtrlLayoutFlag_ResetPreferredSizeOnSerialize);
		ctrlValueBox->setValueKey(SL_SERIALIZEKEY_PLUGIN_INFO_DETAIL);
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

	void PluginInfoDialog::onUIAction( CCObject *sender, CCControlEvent controlEvent )
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

