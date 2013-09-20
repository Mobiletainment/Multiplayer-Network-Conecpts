//!---------------------------------------------------------------------------
//! \file "nlSampleReplicaManager.h"
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
#ifndef __NLSAMPLEREPLICAMANAGER_H__
#define __NLSAMPLEREPLICAMANAGER_H__

namespace nl	{

	class SampleReplicaManager : public ReplicaManager
	{
		SL_DECLARE_BASE(ReplicaManager)
	public:
		SampleReplicaManager(ENetwokTopology topology);
		virtual ~SampleReplicaManager();

		virtual RakNet::Connection_RM3* AllocConnection(
			const RakNet::SystemAddress &systemAddress, 
			RakNet::RakNetGUID rakNetGUID) const SL_OVERRIDE;

		void createReplicas();
		void readReplicas();
		void updateReplicas();
		void deleteReplicas();

	protected:
		virtual RakNet::PluginReceiveResult OnReceive(RakNet::Packet *packet) SL_OVERRIDE;
	private:

	};

}


#endif
