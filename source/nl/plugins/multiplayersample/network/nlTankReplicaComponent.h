//!---------------------------------------------------------------------------
//! \file "nlTankReplicaComponent.h"
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
#ifndef __NLTANKREPLICACOMPONENT_H__
#define __NLTANKREPLICACOMPONENT_H__

#include <stateless/cocosgame/slCocosGame.h>

#include "nlDynamicActorReplicaComponent.h"

namespace nl	{


	class TankReplicaComponent : public DynamicActorReplicaComponent	{
		SL_DECLARE_BASE(DynamicActorReplicaComponent)
	public:
		CREATE_FUNC(TankReplicaComponent)

		SL_IMPLEMENT_CLASSNAME(TankReplicaComponent)

		virtual void preUpdate( float delta ) SL_OVERRIDE;
		virtual void postUpdate( float delta ) SL_OVERRIDE;
		void increaseKillCount();
		void setKillCount(int killCount);

		virtual void preSerialize() SL_OVERRIDE;

		virtual RakNet::RM3SerializationResult serialize(RakNet::SerializeParameters *serializeParameters) SL_OVERRIDE;
		virtual void deserialize(RakNet::DeserializeParameters *deserializeParameters) SL_OVERRIDE;

	protected:
		TankReplicaComponent();
		virtual ~TankReplicaComponent();
		CCLabelTTF* _labelKillCount;
		CCLabelTTF* _labelName;

		SL_SYNTHESIZE(CCString *, _tankName, TankName);
		
	private:
	};


}


#endif // __NLTANKREPLICACOMPONENT_H__
