//!---------------------------------------------------------------------------
//! \file "nlReplicaManagerUIActionDispatcher.cpp"
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
#include "nlReplicaManagerUIActionDispatcher.h"

#include <stateless/cocosgame/slCocosGame.h>

namespace nl	{
	bool ReplicaManagerUIActionDispatcher::init()	{
		bool initialized(SLBaseClass::init());
		return initialized;
	}

	void ReplicaManagerUIActionDispatcher::onUIAction( CCObject *sender, CCControlEvent controlEvent )	{
		// check the replica manager pointer
		// as we do not want to crash here
		GameContentReplicaManager* plugin(_replicaManagerCreator.getReplicaManager());
		if(plugin == nullptr)	{
			SL_PROCESS_APP()->log(ELogType_Error, "a replica manager has not been created");
			return;
		}

		// determine the sender tag to dispatch the action
		const int tag(Dialog::getTag(sender));
		switch(tag)	{
		case SL_CTRLID_REPLICA_CREATE:	{
			plugin->createReplica(GameObjectReplica::staticClassName(), nullptr);
										}
										break;
		case SL_CTRLID_REPLICA_READ:	{
										}
										break;
		case SL_CTRLID_REPLICA_UPDATE:	{
										}
										break;
		case SL_CTRLID_REPLICA_DELETE:	{
										}
										break;
		case SL_CTRLID_REPLICA_LIST:	{
										}
										break;
		}
	}
}


