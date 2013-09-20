//!---------------------------------------------------------------------------
//! \file "slLocalPlayer.cpp"
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
#include "slLocalPlayer.h"
#include "slPedestrianPath.h"

namespace sl	{

	LocalPlayer::LocalPlayer()
		:Player(true)
	{

	}

	void LocalPlayer::setUseLocalController(bool value)	{
		AbstractUpdated* localControler(castToAbstractUpdated(&getLocalController()));
		if(value == true)	{
			accessController()->setCustomUpdated(localControler);
		}
		else	{
			accessController()->setCustomUpdated(nullptr);
		}
	}

	void LocalPlayer::update( const osScalar currentTime, const osScalar elapsedTime )
	{
		// just for debugging
		SLBaseClass::update(currentTime, elapsedTime);
	}

}


