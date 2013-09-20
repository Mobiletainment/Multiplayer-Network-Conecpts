//!---------------------------------------------------------------------------
//! \file "slLocalPlayer.h"
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
#ifndef __SLLOCALPLAYER_H__
#define __SLLOCALPLAYER_H__

#include <stateless/simulation/slOpenSteer.h>

namespace OpenSteer	{
	class Plugin;
}

namespace sl	{


	class slCocosGame_API LocalPlayer : public Player	{
		SL_DECLARE_BASE(Player)
	public:
		LocalPlayer();

		void setUseLocalController(bool value);

		virtual void update( const osScalar currentTime, const osScalar elapsedTime ) SL_OVERRIDE;

		SL_SYNTHESIZE_GET_BY_REF(LocalController, _localController, LocalController)
	private:
	};



}


#endif // __SLLOCALPLAYER_H__
