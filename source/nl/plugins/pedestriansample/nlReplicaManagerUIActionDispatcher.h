//!---------------------------------------------------------------------------
//! \file "nlReplicaManagerUIActionDispatcher.h"
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
#ifndef __NLREPLICAMANAGERUIACTIONDISPATCHER_H__
#define __NLREPLICAMANAGERUIACTIONDISPATCHER_H__

#include "nlGameContentReplicaManager.h"

namespace nl	{


	class ReplicaManagerUIActionDispatcher : public CCNode	{
		SL_DECLARE_BASE(CCNode)
	public:
		CREATE_FUNC(ReplicaManagerUIActionDispatcher)

		virtual bool init();

		void onUIAction( CCObject *sender, CCControlEvent controlEvent );
		
		ReplicaManagerCreator _replicaManagerCreator;
	};


}


#endif // __NLREPLICAMANAGERUIACTIONDISPATCHER_H__
