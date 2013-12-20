

#include "stdafx.h"
#include "nlGameStateReplicaComponent.h"

#include "nlGameContentReplicaManager.h"

#include "nlReplicationRules.h"


namespace nl	{


	//-------------------------------------------------------------------------
	// LPGameStateReplicaComponent
	//-------------------------------------------------------------------------

	nlGameStateReplicaComponent::nlGameStateReplicaComponent() : _connectionCount(0)
		,_notificationConnectionLost(SL_NOTIFY_CONNECTION_LOST)
		,_notificationConnectionDisconnected(SL_NOTIFY_DISCONNECTION)
		,_notificationNewIncommingConnection(SL_NOTIFY_NEW_INCOMMING_CONNECTION)
	{
		_replica.setName(nlGameStateReplicaComponent::staticClassName());
		ServerAuthorityReplicationRule* replicationRule(ServerAuthorityReplicationRule::create());
		replicationRule->_replica = getReplica();
		_replica.setReplicationRule(replicationRule);

		_notificationConnectionLost.addObserver(this, callfuncO_selector(nlGameStateReplicaComponent::onConnectionLostNotification));
		_notificationConnectionDisconnected.addObserver(this, callfuncO_selector(nlGameStateReplicaComponent::onConnectionDisconnectedNotification));
		_notificationNewIncommingConnection.addObserver(this, callfuncO_selector(nlGameStateReplicaComponent::onNewIncommingConnectionNotification));

	}

	nlGameStateReplicaComponent::~nlGameStateReplicaComponent()
	{
		//unregister observed notifications
		_notificationConnectionLost.removeObserver();
		_notificationConnectionDisconnected.removeObserver();
		_notificationNewIncommingConnection.removeObserver();
	}



	void nlGameStateReplicaComponent::preUpdate( float delta ) 	{
		SLBaseClass::preUpdate(delta);
	}

	void nlGameStateReplicaComponent::postUpdate( float delta ) 	{
		SLBaseClass::postUpdate(delta);
	}

	void nlGameStateReplicaComponent::setPeer(Peer* peer) {
		
	}

	void nlGameStateReplicaComponent::onNewIncommingConnectionNotification(CCObject* peerWrapperObject)
	{
		PeerWrapper* peerWrapper(dynamic_cast<PeerWrapper*>(peerWrapperObject));
		if(peerWrapper != nullptr)	
		{
			if(getReplica()->getPeer() == peerWrapper->getPeer())	
			{		
				if(getActiveConnectionCount() >= CONNECTION_LIMIT)
				{
					getReplica()->getPeer()->log(ELogType_Info, "Connection Limit reached, closing connection");	
					getReplica()->getPeer()->accessRakNetPeer()->CloseConnection(peerWrapper->getGUID(), true);
				}
				else
				{
					_connectionList.push_back(peerWrapper->getGUID()); //we maintain this connection ID in a list
					_connectionCount++; //increase the active connection count
				}
			}
		}
	}

	void nlGameStateReplicaComponent::onConnectionLostNotification(CCObject* peerWrapperObject)	
	{
		PeerWrapper* peerWrapper(dynamic_cast<PeerWrapper*>(peerWrapperObject));
		if(peerWrapper != nullptr)	
		{
			removeConnection(peerWrapper);
		}
	}

	void nlGameStateReplicaComponent::onConnectionDisconnectedNotification(CCObject* peerWrapperObject)	
	{
		PeerWrapper* peerWrapper(dynamic_cast<PeerWrapper*>(peerWrapperObject));
		if(peerWrapper != nullptr)	
		{
			removeConnection(peerWrapper);
		}
	}

	void nlGameStateReplicaComponent::removeConnection(PeerWrapper* peerWrapper)
	{
		if(getReplica()->getPeer() == peerWrapper->getPeer())	
		{
			for(int i = 0; i < getActiveConnectionCount(); ++i)
			{
				if(_connectionList[i] == peerWrapper->getGUID())
				{
					_connectionList.erase(_connectionList.begin()+i);
					_connectionCount--;
					break;
				}
			}
		}
	}

	void nlGameStateReplicaComponent::serializeConstruction(RakNet::BitStream *constructionBitstream)	{
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

	bool nlGameStateReplicaComponent::deserializeConstruction(RakNet::BitStream *constructionBitstream)	{
		
		return true;
	}

	void nlGameStateReplicaComponent::serializeDestruction(RakNet::BitStream *destructionBitstream)	{

	}

	bool nlGameStateReplicaComponent::deserializeDestruction(RakNet::BitStream *destructionBitstream)	{
		
		return true;
	}

	// client / authority code
	void nlGameStateReplicaComponent::preSerialize()	{
	}

	// client / authority code
	RakNet::RM3SerializationResult nlGameStateReplicaComponent::serialize(RakNet::SerializeParameters *serializeParameters)	{
		return SLBaseClass::serialize(serializeParameters);
	}

	// server / receive code
	void nlGameStateReplicaComponent::deserialize(RakNet::DeserializeParameters *deserializeParameters)	{
		SLBaseClass::deserialize(deserializeParameters);
	}

}