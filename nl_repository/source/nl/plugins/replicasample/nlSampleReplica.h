//!---------------------------------------------------------------------------
//! \file "nlSampleReplica.h"
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
#ifndef __NLSAMPLEREPLICA_H__
#define __NLSAMPLEREPLICA_H__

namespace nl	{


	class SampleReplica : public AbstractReplica
	{
		SL_DECLARE_BASE(AbstractReplica)
	public:
		SampleReplica();
		~SampleReplica();

		virtual void SerializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *destinationConnection);
		virtual void SerializeDestruction(RakNet::BitStream *destructionBitstream, RakNet::Connection_RM3 *destinationConnection);

		/// Overloaded Replica3 function
		virtual void OnUserReplicaPreSerializeTick(void);

		virtual RakNet::RM3SerializationResult Serialize(RakNet::SerializeParameters *serializeParameters);
		virtual void Deserialize(RakNet::DeserializeParameters *deserializeParameters);

		virtual void OnPoppedConnection(RakNet::Connection_RM3 *droppedConnection);

		// specific implementation
		virtual void notifyReplicaOfMessageDeliveryStatus(RakNet::RakNetGUID guid, uint32_t receiptId, bool messageArrived);
		virtual void log();

		// specific implementation
		void randomizeVariables(void);

	protected:

		// Demonstrate per-variable synchronization
		// We manually test each variable to the last synchronized value and only send those values that change
		int var1Unreliable,var2Unreliable,var3Reliable,var4Reliable;

		// Class to save and compare the states of variables this Serialize() to the last Serialize()
		// If the value is different, true is written to the bitStream, followed by the value. Otherwise false is written.
		// It also tracks which variables changed which Serialize() call, so if an unreliable message was lost (ID_SND_RECEIPT_LOSS) those variables are flagged 'dirty' and resent
		RakNet::VariableDeltaSerializer variableDeltaSerializer;
	private:

	};

}


#endif
