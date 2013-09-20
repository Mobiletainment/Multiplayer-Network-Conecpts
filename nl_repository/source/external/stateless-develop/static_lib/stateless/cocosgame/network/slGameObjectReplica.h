//!---------------------------------------------------------------------------
//! \file "slGameObjectReplica.h"
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
#ifndef __SLGAMEOBJECTREPLICA_H__
#define __SLGAMEOBJECTREPLICA_H__

namespace sl	{

	class ReplicaComponent;

	class slCocosGame_API GameObjectReplica : public AbstractReplica
	{
		SL_DECLARE_BASE(AbstractReplica)
	public:
		GameObjectReplica();
		~GameObjectReplica();

		SL_IMPLEMENT_CLASSNAME(GameObjectReplica)

		virtual void SerializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *destinationConnection);
		virtual bool DeserializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *sourceConnection);
		virtual void SerializeDestruction(RakNet::BitStream *destructionBitstream, RakNet::Connection_RM3 *destinationConnection);
		virtual bool DeserializeDestruction(RakNet::BitStream *destructionBitstream, RakNet::Connection_RM3 *sourceConnection);

		virtual RakNet::RM3ConstructionState QueryConstruction(RakNet::Connection_RM3 *destinationConnection, RakNet::ReplicaManager3 *replicaManager3);
		virtual bool QueryRemoteConstruction(RakNet::Connection_RM3 *sourceConnection);
		virtual RakNet::RM3QuerySerializationResult QuerySerialization(RakNet::Connection_RM3 *destinationConnection);
		virtual RakNet::RM3ActionOnPopConnection QueryActionOnPopConnection(RakNet::Connection_RM3 *droppedConnection) const;

		virtual void DeallocReplica(RakNet::Connection_RM3 *sourceConnection);

		/// Overloaded Replica3 function
		virtual void OnUserReplicaPreSerializeTick(void);

		virtual RakNet::RM3SerializationResult Serialize(RakNet::SerializeParameters *serializeParameters);
		virtual void Deserialize(RakNet::DeserializeParameters *deserializeParameters);

		// specific implementation
		virtual void log();

		virtual void setPeer(Peer* peer) SL_OVERRIDE;

		ReplicaManager* getReplicaManager() const;

		virtual Component* getAbstractReplicaComponent() const SL_OVERRIDE;

		bool isValid() const;

		static void testOffline();

		SL_SYNTHESIZE(ReplicaComponent*, _replicaComponent, ReplicaComponent)

	protected:

	private:

	};

}


#endif
