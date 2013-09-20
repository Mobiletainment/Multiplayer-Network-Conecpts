//!---------------------------------------------------------------------------
//! \file "nlPeerOptionsUI.cpp"
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
#include "nlPeerOptionsUI.h"
#include "nlPeerOptionsDialog.h"

namespace nl	{
	// the peer options UI interface implementation
	void PeerOptionsUI::onUIAction( CCObject *sender, CCControlEvent controlEvent )
	{
		PeerNode* peerNode(getPeerNode());

		const int tag(Dialog::getTag(sender));
		// determine the sender tag to dispatch the action
		if(tag == SL_CTRLID_PEER_CONNECT)	{
		}
		else if(tag == SL_CTRLID_PEER_OPTIONS)	{
			PeerOptionsDialog* dialog(PeerOptionsDialog::createForPeerNode(peerNode));
			if(dialog != nullptr)	{
				dialog->show();
			}
		}
	}

	void PeerOptionsUI::configure()
	{

	}


}


