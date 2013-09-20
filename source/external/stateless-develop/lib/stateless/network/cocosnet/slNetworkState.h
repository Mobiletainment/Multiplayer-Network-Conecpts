//!---------------------------------------------------------------------------
//! \file "slNetworkState.h"
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
#ifndef __SLNETWORKSTATE_H__
#define __SLNETWORKSTATE_H__

#include <stateless/cocosutil/objectstore/slPersistentDictionary.h>

namespace sl	{


	class slNetwork_API NetworkState : public PersistentDictionary
	{
		SL_DECLARE_BASE(PersistentDictionary)
	public:
		CREATE_FUNC(NetworkState)

		static NetworkState* sharedInstance();

		static CCDictionary* getInstanceDictionary(const char* key);


	protected:
		NetworkState();
		virtual ~NetworkState();

	private:
	};

}


#endif
