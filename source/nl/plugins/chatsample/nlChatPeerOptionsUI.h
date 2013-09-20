//!---------------------------------------------------------------------------
//! \file "nlChatPeerOptionsUI.h"
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
#ifndef __NLCHATPEEROPTIONSUI_H__
#define __NLCHATPEEROPTIONSUI_H__

namespace nl	{

	//! @brief ChatPeerOptionsUI extends the basic peer configuration
	//!        with chat specific options
	//!
	class ChatPeerOptionsUI : public PeerOptionsUI, public IDialogExtender	{
		SL_DECLARE_BASE(PeerOptionsUI)
	public:

		CREATE_FUNC(ChatPeerOptionsUI)

		virtual void configure();

		virtual void onUIAction(CCObject *sender, CCControlEvent controlEvent);

		virtual void extendDialog( Dialog* dialog );

	protected:
		~ChatPeerOptionsUI();
	};


}


#endif // __NLCHATPEEROPTIONSUI_H__
