//!---------------------------------------------------------------------------
//! \file "slNetworkState.cpp"
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
#include "slNetworkState.h"
#include <stateless/cocosutil/slCocosUtil.h>

namespace sl	{

	NetworkState* _theInstance(nullptr);

	NetworkState::NetworkState()
	{
		CCAssert(_theInstance == nullptr, "singleton !!!");
		_theInstance = this;
	}

	NetworkState::~NetworkState()	{
		_theInstance = nullptr;
	}

	NetworkState* NetworkState::sharedInstance()
	{
		return _theInstance;
	}

	CCDictionary* NetworkState::getInstanceDictionary(const char* key)	{
		return sharedInstance()->getSubDictionary(key);
	}


}
