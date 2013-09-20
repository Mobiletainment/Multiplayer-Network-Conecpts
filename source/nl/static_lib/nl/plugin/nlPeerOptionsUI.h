//!---------------------------------------------------------------------------
//! \file "nlPeerOptionsUI.h"
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
#ifndef __NLPEEROPTIONSUI_H__
#define __NLPEEROPTIONSUI_H__


namespace nl	{


	//! @brief PeerOptionsUI implements the basic peer configuration options
	//!
	class PeerOptionsUI : public AbstractPeerUI	{
		SL_DECLARE_BASE(AbstractPeerUI)
	public:

		CREATE_FUNC(PeerOptionsUI)

		virtual void configure();

		virtual void onUIAction(CCObject *sender, CCControlEvent controlEvent);

	};


}


#endif // __NLPEEROPTIONSUI_H__
