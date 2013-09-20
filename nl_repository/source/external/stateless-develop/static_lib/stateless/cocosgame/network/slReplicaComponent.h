//!---------------------------------------------------------------------------
//! \file "slReplicaComponent.h"
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
#ifndef __SLREPLICACOMPONENT_H__
#define __SLREPLICACOMPONENT_H__

#include "slGameObjectReplica.h"
#include "actor/slGameActorComponent.h"

namespace sl	{

	class GameplayLayer;
	class ActorSprite;
	class ActorNode;

	class slCocosGame_API ReplicaComponent : public GameActorComponent	{

		friend class GameObjectReplica;

		SL_DECLARE_BASE(GameActorComponent)
	public:
		CREATE_FUNC(ReplicaComponent)

		SL_IMPLEMENT_CLASSNAME(ReplicaComponent)

		virtual bool isValid() const SL_OVERRIDE;

		virtual void preSerialize();

		virtual void serializeConstruction(RakNet::BitStream *constructionBitstream);
		virtual bool deserializeConstruction(RakNet::BitStream *constructionBitstream);

		virtual void serializeDestruction(RakNet::BitStream *destructionBitstream);
		virtual bool deserializeDestruction(RakNet::BitStream *destructionBitstream);


		virtual RakNet::RM3SerializationResult serialize(RakNet::SerializeParameters *serializeParameters);
		virtual void deserialize(RakNet::DeserializeParameters *deserializeParameters);

		GameObjectReplica* getReplica();
		ENetwokTopology getTopology() const;

		virtual void createActorNode(CCDictionary* parameters = nullptr) SL_OVERRIDE;
		virtual void createActorSprite(CCDictionary* parameters = nullptr) SL_OVERRIDE;

		virtual void preUpdate( float delta ) SL_OVERRIDE;
		virtual void postUpdate( float delta ) SL_OVERRIDE;

		virtual void setPeer(Peer* peer) SL_VIRTUAL_DECLARE;

		ReplicaManager* getReplicaManager() const;

		void handleDestroyedActorNode();

		CC_SYNTHESIZE_RETAIN(CCDictionary*, _constructionDictionary, ConstructionDictionary)
	protected:
		ReplicaComponent();
		virtual ~ReplicaComponent();

		virtual void removeFromGameplayLayer() SL_OVERRIDE;

		GameObjectReplica _replica;
		AnimationTick _serializeLogTick;

	};

	SL_INLINE
	GameObjectReplica* ReplicaComponent::getReplica()
	{
		return &_replica;
	}

}


#endif // __SLREPLICACOMPONENT_H__
