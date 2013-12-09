
#pragma once
#ifndef __SLGAMESTATEREPLICACOMPONENT_H__
#define __SLGAMESTATEREPLICACOMPONENT_H__

#include <stateless/cocosgame/slCocosGame.h>

#include "nlProtocolStructures.h"

namespace nl	{

	class GameStateReplicaComponent : public ReplicaComponent
	{
		SL_DECLARE_BASE(ReplicaComponent)
	public:
		CREATE_FUNC(GameStateReplicaComponent)

			SL_IMPLEMENT_CLASSNAME(GameStateReplicaComponent)

			virtual void preUpdate( float delta ) SL_OVERRIDE;
		virtual void postUpdate( float delta ) SL_OVERRIDE;

		virtual void serializeConstruction(RakNet::BitStream *constructionBitstream) SL_OVERRIDE;
		virtual bool deserializeConstruction(RakNet::BitStream *constructionBitstream) SL_OVERRIDE;

		virtual void serializeDestruction(RakNet::BitStream *destructionBitstream) SL_OVERRIDE;
		virtual bool deserializeDestruction(RakNet::BitStream *destructionBitstream) SL_OVERRIDE;

		virtual void preSerialize() SL_OVERRIDE;

		virtual void setPeer(Peer* peer) SL_OVERRIDE;

		virtual RakNet::RM3SerializationResult serialize(RakNet::SerializeParameters *serializeParameters) SL_OVERRIDE;
		virtual void deserialize(RakNet::DeserializeParameters *deserializeParameters) SL_OVERRIDE;

		void onNewIncommingConnectionNotification(CCObject* peerWrapperObject);
		void onConnectionLostNotification(CCObject* peerWrapperObject);
		void onConnectionDisconnectedNotification(CCObject* peerWrapperObject);

		typedef std::vector<RakNet::RakNetGUID> GUIDVector;
		SL_SYNTHESIZE(GUIDVector,_connectionList,GUIDVector);
		SL_SYNTHESIZE(int,_numConnections,NumberOfConnections); 

	protected:
		GameStateReplicaComponent();
		virtual ~GameStateReplicaComponent();

		int _num;
	private:
		Notification _notificationConnectionLost;
		Notification _notificationConnectionDisconnected;
		Notification _notificationNewIncommingConnection;
	};

}


#endif // __SLGAMESTATEREPLICACOMPONENT_H__

