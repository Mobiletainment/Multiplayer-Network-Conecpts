//!---------------------------------------------------------------------------
//! \file "slPeerNode.cpp"
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
#include "slPeerNode.h"
#include "slPeerNodeControLayer.h"
#include <stateless/network/replica/slAbstractReplica.h>
#include <stateless/framework/slFramework.h>
#include <stateless/cocosutil/slCocosUtil.h>
#include "slNetworkState.h"

using namespace RakNet;


namespace sl	{


	PeerNode* AbstractPeerUI::getPeerNode() const
	{
		return dynamic_cast<PeerNode*>(getOwner());
	}

	AbstractNodeArray PeerNode::_allPeerNodes;

	PeerNode::PeerNode()
		:_numberOfConnections(0)
		,_btnConnect(nullptr)
		,_ctrlClientPort(nullptr)
		,_ctrlPeerToPeerPort(nullptr)
		,_ctrlServerPort(nullptr)
		,_ctrlConnections(nullptr)
		,_ctrlLocalIP(nullptr)
	{
		_allPeerNodes.add(this);
		setPeer(&_peerInstance);
		_netPacketDispatchers = new CCArray();
		_netPacketSenders = new CCArray();
		_uiInterfaces = new CCArray();

		CCNotificationCenter* notificationCenter(CCNotificationCenter::sharedNotificationCenter());
		notificationCenter->addObserver(this, callfuncO_selector(PeerNode::onPeerStateChanged), SL_NOTIFY_PEERSTATE_CHANGED, NULL );
		notificationCenter->addObserver(this, callfuncO_selector(PeerNode::onPeerStateChanged), SL_NOTIFY_PEERCONNECTION_CHANGED, NULL );
	}

	PeerNode::~PeerNode()	{
		CCNotificationCenter* notificationCenter(CCNotificationCenter::sharedNotificationCenter());
		notificationCenter->removeObserver(this, SL_NOTIFY_PEERSTATE_CHANGED);
		notificationCenter->removeObserver(this, SL_NOTIFY_PEERCONNECTION_CHANGED);

		_peerInstance.shutdown();
		CC_SAFE_DELETE(_netPacketDispatchers);
		CC_SAFE_DELETE(_netPacketSenders);
		CC_SAFE_DELETE(_uiInterfaces);
		setBtnConnect(nullptr);
		setPeer(nullptr);
		_allPeerNodes.remove(this);
	}

	void PeerNode::shutDownAllPeers()	{
		SLSize idx(0);
		PeerNode* peerNode(dynamic_cast<PeerNode*>(_allPeerNodes.nodeAt(idx)));
		while(peerNode != nullptr)	{
			const bool peerConnected(peerNode->getPeer()->isConnected());
			// this will lead to shutting down this peer
			if(peerConnected)	{
				peerNode->getPeer()->setTryConnect(false);
			}
			++idx;
			peerNode = dynamic_cast<PeerNode*>(_allPeerNodes.nodeAt(idx));
		}
	}

	SLSize PeerNode::getPeerNodesCount()	{
		return _allPeerNodes.nodeCount();
	}

	const AbstractNodeArray& PeerNode::getAllPeerNodes()	{
		return _allPeerNodes;
	}

	bool PeerNode::init()	{
		bool initialized(SLBaseClass::init());
		if(initialized)	{
			int netWorkUpdatePriority(0);

			CCString* configurationRootKey;
			if(_peerInstance.isServer())	{
				// assign a human readable data key
				configurationRootKey = CCString::createWithFormat("server_%d", peerIdx());

				// note: we want the server network to be updated last
				//       so we assign a high priority here
				//       see cocos documentation
				netWorkUpdatePriority = (INT_MAX - 1000);
			}
			else if(_peerInstance.isClient())	{
				// assign a human readable data key
				configurationRootKey = CCString::createWithFormat("client_%d", peerIdx());

				// note: we want the client network to be updated first
				//       so we assign a low priority here
				//       see cocos documentation
				netWorkUpdatePriority = (INT_MIN + 1000);
			}
			else if(_peerInstance.isPeerToPeer())	{
				// assign a human readable data key
				configurationRootKey = CCString::createWithFormat("peerToPeer_%d", peerIdx());

				// note: we want the peer to peer network to be updated first
				//       so we assign a low priority here
				//       see cocos documentation
				netWorkUpdatePriority = (INT_MIN + 1000);
			}
			_peerInstance.setConfigurationRootKey(configurationRootKey->getCString());

			CCDictionary* peerSetting(_peerInstance.getPeerConfiguration());
			if(peerSetting == nullptr)	{
				_peerInstance.log(ELogType_Error,"failed to retrieve peer configuration");
				initialized = false;
			}
			else	{
				// configure once
				Dictionary::setObjectIfNotExist(peerSetting, CCInteger::create(60000),SL_SERIALIZEKEY_PEER_SERVER_PORT);
				Dictionary::setObjectIfNotExist(peerSetting, CCString::create("127.0.0.1"),SL_SERIALIZEKEY_PEER_SERVERIP);
				Dictionary::setObjectIfNotExist(peerSetting, CCString::create("<unknown>"),SL_SERIALIZEKEY_PEER_LOCALIP);
				Dictionary::setObjectIfNotExist(peerSetting, CCInteger::create(51000),SL_SERIALIZEKEY_PEER_CLIENT_PORT);
				Dictionary::setObjectIfNotExist(peerSetting, CCInteger::create(55000),SL_SERIALIZEKEY_PEER_P2P_PORT);
				Dictionary::setObjectIfNotExist(peerSetting, CCInteger::create(0),SL_SERIALIZEKEY_PEER_CONNECTIONS);
				Dictionary::setObjectIfNotExist(peerSetting, CCBool::create(false),SL_SERIALIZEKEY_PEER_USE_PACKETLOGGER);
				Dictionary::setObjectIfNotExist(peerSetting, CCBool::create(false),SL_SERIALIZEKEY_PEER_USE_FULLYCONNECTEDMESH);
				Dictionary::setObjectIfNotExist(peerSetting, CCBool::create(false),SL_SERIALIZEKEY_PEER_USE_RPC4);
				Dictionary::setObjectIfNotExist(peerSetting, CCBool::create(false),SL_SERIALIZEKEY_PEER_USE_REPLICAMANAGER);
				Dictionary::setObjectIfNotExist(peerSetting, CCBool::create(false),SL_SERIALIZEKEY_PEER_USE_REPLICAMANAGER);
				Dictionary::setObjectIfNotExist(peerSetting, CCBool::create(false),SL_SERIALIZEKEY_PEER_ADVERTISE_SYSTEM);

				// note: referring to valid port lists
				//       ports above 49152 are not officially designated to any service
				//       so we take smth above ...

				// validate allowed ports
				int serverPort(Dictionary::getInteger(peerSetting, SL_SERIALIZEKEY_PEER_SERVER_PORT, 60000));
				if(serverPort < 51000)	{
					peerSetting->setObject(CCInteger::create(60000),SL_SERIALIZEKEY_PEER_SERVER_PORT);
				}
				int clientPort(Dictionary::getInteger(peerSetting, SL_SERIALIZEKEY_PEER_CLIENT_PORT, 50000));
				if(clientPort < 51000)	{
					peerSetting->setObject(CCInteger::create(51000),SL_SERIALIZEKEY_PEER_CLIENT_PORT);
				}
				int peerToPeerPort(Dictionary::getInteger(peerSetting, SL_SERIALIZEKEY_PEER_P2P_PORT, 55000));
				if(peerToPeerPort < 51000)	{
					peerSetting->setObject(CCInteger::create(55000),SL_SERIALIZEKEY_PEER_P2P_PORT);
				}

				// this leads to update being called once per frame
				scheduleUpdateWithPriority(netWorkUpdatePriority);
			}
		}
		return initialized;
	}

	void PeerNode::update( float dt )	{
		// in debug mode read the network simulator settings
#ifdef _DEBUG
		// TODO: @CP - improve performance
		CCDictionary* peerSetting(_peerInstance.getPeerConfiguration());
		if(peerSetting == nullptr)	{
			_peerInstance.log(ELogType_Error,"failed to retrieve peer configuration");
		}
		else	{
			NetworkSimulatorSetting networkSimulatorSettings;
			networkSimulatorSettings.enabled = Dictionary::getBool(peerSetting, SL_SERIALIZEKEY_PEER_NETWORKSIMULATOR_ENABLE);
			networkSimulatorSettings.packetloss = Dictionary::getFloat(peerSetting, SL_SERIALIZEKEY_PEER_NETWORKSIMULATOR_PACKETLOSS);
			networkSimulatorSettings.minExtraPing = Dictionary::getInteger(peerSetting, SL_SERIALIZEKEY_PEER_NETWORKSIMULATOR_MIN_EXTRA_PING);
			networkSimulatorSettings.extraPingVariance = Dictionary::getInteger(peerSetting, SL_SERIALIZEKEY_PEER_NETWORKSIMULATOR_EXTRA_PING_VARIANCE);
			_peerInstance.updateNetworkSimulatorSetting(networkSimulatorSettings);
		}
#endif
		// note: we update the network before anything else here
		updateNetwork(dt);
		SLBaseClass::update(dt);
	}

	void PeerNode::onConnectAction(CCObject *sender, CCControlEvent controlEvent)	{
		// grab the connect button
		CCControlButton* btn(dynamic_cast<CCControlButton*>(sender));
		if(getBtnConnect() != btn)	{
			setBtnConnect(btn);
		}

		// track if the connection status has changed
		const bool peerConnected(_peerInstance.isConnected());
		if(peerConnected)	{
			_peerInstance.setTryConnect(false);
		}
		else	{
			_peerInstance.setTryConnect(true);
		}

		if(peerConnected != _peerInstance.isConnected())	{
			CCNotificationCenter* notificationCenter(CCNotificationCenter::sharedNotificationCenter());
			notificationCenter->postNotification(SL_NOTIFY_PEERSTATE_CHANGED, this);
		}
	}

	void PeerNode::dispatchPacket(unsigned char packetIdentifier, NativePacket* nativePacket)	{
		CCObject* obj;
		CCARRAY_FOREACH(_netPacketDispatchers, obj)	{
			INetPacketDispatcher* packetDispatcher(dynamic_cast<INetPacketDispatcher*>(obj));
			if(packetDispatcher != nullptr)	{
				packetDispatcher->dispatchPacket(packetIdentifier, nativePacket);
			}
		}
	}

	void PeerNode::updateNetwork(float dt)	{
		const unsigned short numberOfConnections(_peerInstance.getNumberOfConnections());

		// track if the connection status has changed
		const bool peerConnected(_peerInstance.isConnected());
		if(peerConnected)	{
			// in case connected attached net packet senders can do their job
			// meaning to serialize data to connected peers
			CCObject* obj;
			CCARRAY_FOREACH(_netPacketSenders, obj)	{
				INetPacketSender* packetSender(dynamic_cast<INetPacketSender*>(obj));
				if(packetSender != nullptr)	{
					packetSender->serialize(dt);
				}
			}
		}

		// now the peer instance itself can do it's network job
		_peerInstance.updateNetwork(dt, this);

		// notify those who want to know about status changes of this peer node
		if(_numberOfConnections != numberOfConnections)	{
			_numberOfConnections = numberOfConnections;
			CCNotificationCenter* notificationCenter(CCNotificationCenter::sharedNotificationCenter());
			notificationCenter->postNotification(SL_NOTIFY_PEERCONNECTION_CHANGED, this);	
		}

		if(peerConnected != _peerInstance.isConnected())	{
			CCNotificationCenter* notificationCenter(CCNotificationCenter::sharedNotificationCenter());
			notificationCenter->postNotification(SL_NOTIFY_PEERSTATE_CHANGED, this);	
		}
	}

	void PeerNode::createPeerUI(CCLayer* parentLayer)	{
		if(parentLayer == nullptr)	{
			return;
		}
		else	{
			PeerNodeControlLayer::addPeerControls(parentLayer,this);
		}
	}

	void PeerNode::onEnter()	{
		SLBaseClass::onEnter();

		// TODO: right now we are abusing CCComponent ...
		//       never ever call addComponent on a cocos2d node
		//       addComponent is doing pretty ugly stuff
		CCObject* obj;
		CCARRAY_FOREACH(_uiInterfaces, obj)	{
			AbstractPeerUI* abstractPeerUI(dynamic_cast<AbstractPeerUI*>(obj));
			if(abstractPeerUI != nullptr)	{
				abstractPeerUI->setOwner(this);
				abstractPeerUI->onEnter();
				abstractPeerUI->setOwner(nullptr);
			}
		}

	}

	void PeerNode::onExit()	{
		SLBaseClass::onExit();
	}

	void PeerNode::onPeerStateChanged(CCObject* sender)	{
		if(sender == this)	{
			const Peer& peer(_peerInstance);

			CCDictionary* peerSetting(peer.getPeerConfiguration());
			if(peerSetting == nullptr)	{
				peer.log(ELogType_Error,"failed to retrieve peer configuration");
				return;
			}

			int rakConnections = peer.getNumberOfConnections();
			CCInteger* connections(dynamic_cast<CCInteger*>(peerSetting->objectForKey(SL_SERIALIZEKEY_PEER_CONNECTIONS)));
			if(connections != nullptr)	{
				if(rakConnections != connections->getValue())	{
					peerSetting->setObject(CCInteger::create(rakConnections),SL_SERIALIZEKEY_PEER_CONNECTIONS);
					if(_ctrlConnections != nullptr)	{
						_ctrlConnections->deserialize(peerSetting);
					}
				}
			}

			SLAString peerInfo("");
			if(peer.isConnected())	{
				CCControlButton* btn(getBtnConnect());
				if(btn != nullptr)	{
					if(_peerInstance.isClient())	{
						btn->setTitleForState(ccs("Disconnect"), CCControlStateNormal);
					}
					else	{
						btn->setTitleForState(ccs("Stop"), CCControlStateNormal);
					}
					btn->setTitleColorForState(ccRED, CCControlStateNormal);
					CCControlBase* parent(dynamic_cast<CCControlBase*>(btn->getParent()));
					if(parent != nullptr)	{
						CCSize preferredSize(parent->getPreferredSize());
						preferredSize.width = 0;
						parent->setPreferredSize(preferredSize);
						parent->needsLayout();
					}
				}

				// the server or the client port might have changed
				// read back to the NetworkState and update the ui
				const NetworkAddress& networkAdress(_peerInstance.getNetworkAddress());
				CCInteger* serverPort(dynamic_cast<CCInteger*>(peerSetting->objectForKey(SL_SERIALIZEKEY_PEER_SERVER_PORT)));
				if(serverPort != nullptr)	{
					if(networkAdress.getServerPort() != serverPort->getValue())	{
						peerSetting->setObject(CCInteger::create(networkAdress.getServerPort()),SL_SERIALIZEKEY_PEER_SERVER_PORT);
						if(_ctrlServerPort != nullptr)	{
							_ctrlServerPort->deserialize(peerSetting);
						}
					}
				}

				CCInteger* clientPort(dynamic_cast<CCInteger*>(peerSetting->objectForKey(SL_SERIALIZEKEY_PEER_CLIENT_PORT)));
				if(clientPort != nullptr)	{
					if(networkAdress.getClientPort() != clientPort->getValue())	{
						peerSetting->setObject(CCInteger::create(networkAdress.getClientPort()),SL_SERIALIZEKEY_PEER_CLIENT_PORT);
						if(_ctrlClientPort != nullptr)	{
							_ctrlClientPort->deserialize(peerSetting);
						}
					}
				}

				CCInteger* peerToPeerPort(dynamic_cast<CCInteger*>(peerSetting->objectForKey(SL_SERIALIZEKEY_PEER_P2P_PORT)));
				if(peerToPeerPort != nullptr)	{
					if(networkAdress.getPeerToPeerPort() != peerToPeerPort->getValue())	{
						peerSetting->setObject(CCInteger::create(networkAdress.getPeerToPeerPort()),SL_SERIALIZEKEY_PEER_P2P_PORT);
						if(_ctrlPeerToPeerPort != nullptr)	{
							_ctrlPeerToPeerPort->deserialize(peerSetting);
						}
					}
				}

				// TODO: support more than one local address
				RakNet::RakPeerInterface* raknetPeer = peer.accessRakNetPeer();
				const unsigned numberOfAdresses(raknetPeer->GetNumberOfAddresses());
				_peerInstance.log(ELogType_Info, "numberOfAdresses: %i", numberOfAdresses);
				for (unsigned i=0; i < numberOfAdresses; ++i)
				{
					if(i == 0)	{
						peerSetting->setObject(CCString::create(raknetPeer->GetLocalIP(i)),SL_SERIALIZEKEY_PEER_LOCALIP);
						if(_ctrlLocalIP != nullptr)	{
							_ctrlLocalIP->deserialize(peerSetting);
						}
					}
					_peerInstance.log(ELogType_Info, "LocalIP: %i %s", i+1, raknetPeer->GetLocalIP(i));
				}
				_peerInstance.log(ELogType_Info, "GUID: %s", peer.getPeerGUID().ToString());
			}
			else	{
				// not connected
				CCControlButton* btn(getBtnConnect());
				if(btn != nullptr)	{
					if(_peerInstance.isClient())	{
						btn->setTitleForState(ccs("Connect"), CCControlStateNormal);
					}
					else	{
						btn->setTitleForState(ccs("Start"), CCControlStateNormal);
					}
					btn->setTitleColorForState(ccGREEN, CCControlStateNormal);
					CCControlBase* parent(dynamic_cast<CCControlBase*>(btn->getParent()));
					if(parent != nullptr)	{
						parent->needsLayout();
					}
				}
			}
		}
	}

	void PeerNode::addNetPacketDispatcher( CCObject* packetDispatcher )	{
		// first check if this object is a packet dispatcher
		INetPacketDispatcher* packetDispatcherInterface(dynamic_cast<INetPacketDispatcher*>(packetDispatcher));
		if(packetDispatcherInterface != nullptr)	{
			if(_netPacketDispatchers->containsObject(packetDispatcher) == false)	{
				packetDispatcherInterface->setPeer(getPeer());
				_netPacketDispatchers->addObject(packetDispatcher);
			}
		}
	}

	void PeerNode::addNetPacketSender( CCObject* netPacketSender )	{
		// first check if this object is a net data sender
		INetPacketSender* netPacketSenderInterface(dynamic_cast<INetPacketSender*>(netPacketSender));
		if(netPacketSenderInterface != nullptr)	{
			if(_netPacketSenders->containsObject(netPacketSender) == false)	{
				netPacketSenderInterface->setPeer(getPeer());
				_netPacketSenders->addObject(netPacketSender);
			}
		}
	}

	void PeerNode::onUIAction( CCObject *sender, CCControlEvent controlEvent )	{
		CCObject* obj;
		CCARRAY_FOREACH(_uiInterfaces, obj)	{
			AbstractPeerUI* abstractPeerUI(dynamic_cast<AbstractPeerUI*>(obj));
			if(abstractPeerUI != nullptr)	{
				abstractPeerUI->setOwner(this);
				abstractPeerUI->onUIAction( sender, controlEvent );
				abstractPeerUI->setOwner(nullptr);
			}
		}
	}

	void PeerNode::addUI( CCObject* uiInterface )	{
		// first check if this object is a AbstractPeerUI
		AbstractPeerUI* abstractPeerUI(dynamic_cast<AbstractPeerUI*>(uiInterface));
		if(abstractPeerUI != nullptr)	{
			if(_uiInterfaces->containsObject(uiInterface) == false)	{
				_uiInterfaces->addObject(uiInterface);
			}
		}
	}

	CCControlBase* PeerNode::createPeerInfoControl()	{
		CCSize btnContainerSize(CCSizeMake(0,0));
		CCArray* columnCtrls = CCArray::create();

		if(_peerInstance.isServer())	{
			CCSprite* sprite(CCSprite::create("network/server.png"));
			const CCSize& spriteSize(sprite->getContentSize());
			sprite->setContentSize(CCSizeMake(
				spriteSize.width * 0.7f,
				spriteSize.height * 0.7f));
			columnCtrls->addObject(sprite);
			CCNode* lblPeerType(ControlUtils::createLabel(
				CCString::createWithFormat("Server-%d", peerIdx())->getCString()));
			columnCtrls->addObject(lblPeerType);
		}
		else if(_peerInstance.isClient())	{
			CCSprite* sprite(CCSprite::create("network/client.png"));
			const CCSize& spriteSize(sprite->getContentSize());
			sprite->setContentSize(CCSizeMake(
				spriteSize.width * 0.7f,
				spriteSize.height * 0.7f));
			columnCtrls->addObject(sprite);
			CCNode* lblPeerType(ControlUtils::createLabel(
				CCString::createWithFormat("Client-%d", peerIdx())->getCString()));
			columnCtrls->addObject(lblPeerType);
		}
		else if(_peerInstance.isPeerToPeer())	{
			CCSprite* sprite(CCSprite::create("network/client.png"));
			const CCSize& spriteSize(sprite->getContentSize());
			sprite->setContentSize(CCSizeMake(
				spriteSize.width * 0.7f,
				spriteSize.height * 0.7f));
			columnCtrls->addObject(sprite);
			CCNode* lblPeerType(ControlUtils::createLabel(
				CCString::createWithFormat("P2Peer-%d", peerIdx())->getCString()));
			columnCtrls->addObject(lblPeerType);
		}

		CCControlColumn* btnContainer(ControlUtils::createControlColumn(columnCtrls,btnContainerSize));
		btnContainer->getCtrlFlags().removeFlag(ECtrlFlag_UseScissorTest);
		return btnContainer;
	}
}
