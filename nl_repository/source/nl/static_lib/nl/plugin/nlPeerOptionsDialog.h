//!---------------------------------------------------------------------------
//! \file "nlPeerOptionsDialog.h"
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
#ifndef __NLPEEROPTIONSDIALOG_H__
#define __NLPEEROPTIONSDIALOG_H__


namespace nl	{


	class PeerOptionsDialog : public Dialog	{
		SL_DECLARE_BASE(Dialog)
	public:
		bool initWidthPeerNode(PeerNode* peerNode, IDialogExtender* dialogExtender = nullptr);

		virtual void onUIAction(CCObject *sender, CCControlEvent controlEvent);

		static PeerOptionsDialog* createForPeerNode(PeerNode* peerNode, IDialogExtender* dialogExtender = nullptr);
	protected:
		virtual ~PeerOptionsDialog()	{

		}
	};



}


#endif // __NLPEEROPTIONSDIALOG_H__
