

#include "stdafx.h"
#include "GameStateReplicaComponent.h"

#include "nlGameContentReplicaManager.h"

#include "nlReplicationRules.h"


namespace nl	{


	//-------------------------------------------------------------------------
	// GameStateReplicaComponent
	//-------------------------------------------------------------------------

	GameStateReplicaComponent::GameStateReplicaComponent()
		:_num(0)
		,_numConnections(0)
		,_notificationConnectionLost(SL_NOTIFY_CONNECTION_LOST)
		,_notificationConnectionDisconnected(SL_NOTIFY_DISCONNECTION)
		,_notificationNewIncommingConnection(SL_NOTIFY_NEW_INCOMMING_CONNECTION)
	{
		_replica.setName(GameStateReplicaComponent::staticClassName());
		ServerAuthorityReplicationRule* replicationRule(ServerAuthorityReplicationRule::create());
		replicationRule->_replica = getReplica();
		_replica.setReplicationRule(replicationRule);

		_notificationConnectionLost.addObserver(this, callfuncO_selector(GameStateReplicaComponent::onConnectionLostNotification));
		_notificationConnectionDisconnected.addObserver(this, callfuncO_selector(GameStateReplicaComponent::onConnectionDisconnectedNotification));
		_notificationNewIncommingConnection.addObserver(this, callfuncO_selector(GameStateReplicaComponent::onNewIncommingConnectionNotification));

	}

	GameStateReplicaComponent::~GameStateReplicaComponent()	{
		_notificationConnectionLost.removeObserver();
		_notificationConnectionDisconnected.removeObserver();
		_notificationNewIncommingConnection.removeObserver();
	}



	void GameStateReplicaComponent::preUpdate( float delta ) 	{
		SLBaseClass::preUpdate(delta);
	}

	void GameStateReplicaComponent::postUpdate( float delta ) 	{
		SLBaseClass::postUpdate(delta);
	}

	void GameStateReplicaComponent::setPeer(Peer* peer) {
		
	}

	void GameStateReplicaComponent::onNewIncommingConnectionNotification(CCObject* peerWrapperObject)
	{
		PeerWrapper* peerWrapper(dynamic_cast<PeerWrapper*>(peerWrapperObject));
		if(peerWrapper != nullptr)	
		{
			if(getReplica()->getPeer() == peerWrapper->getPeer())	
			{
				if(_numConnections>=3)
				{
					getReplica()->getPeer()->accessRakNetPeer()->CloseConnection(peerWrapper->getGUID(), true);
				}
				else
				{
					_numConnections++;
					_connectionList.push_back(peerWrapper->getGUID());
				}
			}
		}
	}

	void GameStateReplicaComponent::onConnectionLostNotification(CCObject* peerWrapperObject)	
	{
		PeerWrapper* peerWrapper(dynamic_cast<PeerWrapper*>(peerWrapperObject));
		if(peerWrapper != nullptr)	
		{
			if(getReplica()->getPeer() == peerWrapper->getPeer())	
			{
				for(int i = 0; i< _numConnections; i++)
				{
					if(_connectionList[i] == peerWrapper->getGUID())
					{
						_connectionList.erase(_connectionList.begin()+i);
						_numConnections--;
						break;
					}
				}
			}
		}
	}

	void GameStateReplicaComponent::onConnectionDisconnectedNotification(CCObject* peerWrapperObject)	
	{
		PeerWrapper* peerWrapper(dynamic_cast<PeerWrapper*>(peerWrapperObject));
		if(peerWrapper != nullptr)	
		{
			if(getReplica()->getPeer() == peerWrapper->getPeer())	
			{
				for(int i = 0; i< _numConnections; i++)
				{
					if(_connectionList[i] == peerWrapper->getGUID())
					{
						_connectionList.erase(_connectionList.begin()+i);
						_numConnections--;
						break;
					}
				}
			}
		}
	}

	void GameStateReplicaComponent::serializeConstruction(RakNet::BitStream *constructionBitstream)	{
		CCDictionary* constructionDictionary(getConstructionDictionary());
		if(constructionDictionary != nullptr)	{
			// the vehicle position and orientation might have changed
			// so we need to perform an update here
			AbstractVehicle* vehicle(getActorSprite()->getVehicle());
			if(vehicle != nullptr)	{
				// TODO @student : modify the construction dictionary to spawn at the right place, with the right direction and right initial velocities

			}
			CCString* constructionJSON = CCJSONConverter::strFrom(constructionDictionary);
			constructionBitstream->Write(constructionJSON->getCString());

			getReplica()->getPeer()->log(ELogType_Info, "%s - Dictionary", constructionJSON->getCString());	
		}
	}

	bool GameStateReplicaComponent::deserializeConstruction(RakNet::BitStream *constructionBitstream)	{
		
		return true;
	}

	void GameStateReplicaComponent::serializeDestruction(RakNet::BitStream *destructionBitstream)	{

	}

	bool GameStateReplicaComponent::deserializeDestruction(RakNet::BitStream *destructionBitstream)	{
		
		return true;
	}

	// client / authority code
	void GameStateReplicaComponent::preSerialize()	{
	}

	// client / authority code
	RakNet::RM3SerializationResult GameStateReplicaComponent::serialize(RakNet::SerializeParameters *serializeParameters)	{
		return SLBaseClass::serialize(serializeParameters);
	}

	// server / receive code
	void GameStateReplicaComponent::deserialize(RakNet::DeserializeParameters *deserializeParameters)	{
		SLBaseClass::deserialize(deserializeParameters);
	}

}