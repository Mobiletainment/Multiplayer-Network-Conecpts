//!---------------------------------------------------------------------------
//! \file "slNetworkAddress.h"
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
#ifndef __SLNETWORKADDRESS_H__
#define __SLNETWORKADDRESS_H__

#include "slNetworkEnums.h"

#pragma warning (push)
#pragma warning (disable : 4251)


namespace sl	{

	class slNetwork_API NetworkAddress
	{
	public:
		NetworkAddress();
		virtual ~NetworkAddress();

		const char* getServerIp( ) const;
		const char* getSessionPassword( ) const;
		unsigned short getClientPort( ) const;
		unsigned short getPeerToPeerPort( ) const;
		unsigned short getServerPort( ) const;

		void setServerIp(const char* ip);
		void setClientPort(unsigned short port);
		void setPeerToPeerPort(unsigned short port);
		void setServerPort(unsigned short port);

		void setSessionPassword(const char* pw);

		void getString( ENetwokTopology topoloy, SLAString& out ) const;
	protected:


	private:
		SLAString _serverIp;
		SLAString _sessionPassword;
		unsigned short _clientPort;
		unsigned short _peerToPeerPort;
		unsigned short _serverPort;
	};

}

#pragma warning (pop)

#endif
