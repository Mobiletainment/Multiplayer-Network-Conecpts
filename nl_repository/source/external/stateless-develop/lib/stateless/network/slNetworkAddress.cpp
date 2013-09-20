//!---------------------------------------------------------------------------
//! \file "slNetworkAddress.cpp"
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
#include "slNetworkAddress.h"

namespace sl	{



	NetworkAddress::NetworkAddress()
		:_clientPort(40000)
		,_peerToPeerPort(45000)
		,_serverPort(50000)
	{
		_serverIp = "127.0.0.1";
		// TODO: randomize or setting
		_sessionPassword = "Rumpelstiltskin";
	}

	NetworkAddress::~NetworkAddress()
	{

	}

	void NetworkAddress::getString( ENetwokTopology topoloy, SLAString& out ) const	{
		std::ostringstream messageString;
		switch(topoloy)	{
		case(CLIENT):	{
			messageString << "127.0.0.1" << "|" << _clientPort;
						}
						break;
		case(P2P):	{
			messageString << "127.0.0.1" << "|" << _peerToPeerPort;
						}
						break;
		case(SERVER):	{
			messageString << _serverIp << "|" << _serverPort;
						}
						break;
		}
		out = messageString.str();
	}

	const char* NetworkAddress::getServerIp() const
	{
		return _serverIp.c_str();
	}

	unsigned short NetworkAddress::getClientPort() const
	{
		return _clientPort;
	}

	unsigned short NetworkAddress::getPeerToPeerPort() const
	{
		return _peerToPeerPort;
	}

	unsigned short NetworkAddress::getServerPort() const
	{
		return _serverPort;
	}

	void NetworkAddress::setServerIp( const char* ip )
	{
		_serverIp = ip;
	}

	void NetworkAddress::setClientPort( unsigned short port )
	{
		_clientPort = port;
	}

	void NetworkAddress::setPeerToPeerPort( unsigned short port )
	{
		_peerToPeerPort = port;
	}

	void NetworkAddress::setServerPort( unsigned short port )
	{
		_serverPort = port;
	}

	const char* NetworkAddress::getSessionPassword() const
	{
		return _sessionPassword.c_str();
	}

	void NetworkAddress::setSessionPassword( const char* pw )
	{
		_sessionPassword.assign(pw);
	}

}


