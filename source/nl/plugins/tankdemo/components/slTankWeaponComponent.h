//!---------------------------------------------------------------------------
//! \file "slTankWeaponComponent.h"
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
#ifndef __SLTANKWEAPONCOMPONENT_H__
#define __SLTANKWEAPONCOMPONENT_H__

#include <stateless/cocosgame/slCocosGame.h>

namespace sl	{


	class TankWeaponComponent : public GameActorComponent	{
		SL_DECLARE_BASE(GameActorComponent)
	public:
		CREATE_FUNC(TankWeaponComponent)

		virtual void preUpdate( float delta ) SL_OVERRIDE;
		virtual void postUpdate( float delta ) SL_OVERRIDE;

	protected:
		TankWeaponComponent();
		virtual ~TankWeaponComponent();

		AnimationTick _fireTick;
		SLSize _updateTick;
	};

}


#endif // __SLTANKWEAPONCOMPONENT_H__
