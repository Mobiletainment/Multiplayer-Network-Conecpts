//!---------------------------------------------------------------------------
//! \file "nlTankProjectileReplicaComponent.h"
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
#ifndef __NLTANKPROJECTILEREPLICACOMPONENT_H__
#define __NLTANKPROJECTILEREPLICACOMPONENT_H__

#include <stateless/cocosgame/slCocosGame.h>

#include "nlDynamicActorReplicaComponent.h"

namespace nl	{


	class TankProjectileReplicaComponent : public DynamicActorReplicaComponent	{
		SL_DECLARE_BASE(DynamicActorReplicaComponent)
	public:
		CREATE_FUNC(TankProjectileReplicaComponent)

		SL_IMPLEMENT_CLASSNAME(TankProjectileReplicaComponent)

		virtual void preUpdate( float delta ) SL_OVERRIDE;
		virtual void postUpdate( float delta ) SL_OVERRIDE;

	protected:
		TankProjectileReplicaComponent();
		virtual ~TankProjectileReplicaComponent();

		AnimationTick _lifeTimeTick;
		Player _player;

	private:

	};


}


#endif // __NLTANKPROJECTILEREPLICACOMPONENT_H__
