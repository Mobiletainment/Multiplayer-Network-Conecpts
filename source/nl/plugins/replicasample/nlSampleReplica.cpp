//!---------------------------------------------------------------------------
//! \file "nlSampleReplica.cpp"
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
#include "nlSampleReplica.h"

#include "RakString.h"
#include "Rand.h"

namespace nl	{
	SampleReplica::SampleReplica()
		:var1Unreliable(0)
		,var2Unreliable(0)
		,var3Reliable(0)
		,var4Reliable(0)
	{
	}

	SampleReplica::~SampleReplica()	{
	}

	void SampleReplica::SerializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *destinationConnection)	{
		// variableDeltaSerializer is a helper class that tracks what variables were sent to what remote system
		// This call adds another remote system to track
		variableDeltaSerializer.AddRemoteSystemVariableHistory(destinationConnection->GetRakNetGUID());
		constructionBitstream->Write(getName() + RakNet::RakString(" SerializeConstruction"));
	}

	void SampleReplica::SerializeDestruction(RakNet::BitStream *destructionBitstream, RakNet::Connection_RM3 *destinationConnection)	{
		// variableDeltaSerializer is a helper class that tracks what variables were sent to what remote system
		// This call removes a remote system
		variableDeltaSerializer.RemoveRemoteSystemVariableHistory(destinationConnection->GetRakNetGUID());
		destructionBitstream->Write(getName() + RakNet::RakString(" SerializeDestruction"));
	}

	/// Overloaded Replica3 function
	void SampleReplica::OnUserReplicaPreSerializeTick(void)	{
		/// Required by VariableDeltaSerializer::BeginIdenticalSerialize()
		variableDeltaSerializer.OnPreSerializeTick();
	}

	RM3SerializationResult SampleReplica::Serialize(SerializeParameters *serializeParameters)	{
		VariableDeltaSerializer::SerializationContext serializationContext;

		// Put all variables to be sent unreliably on the same channel, then specify the send type for that channel
		serializeParameters->pro[0].reliability=UNRELIABLE_WITH_ACK_RECEIPT;
		// Sending unreliably with an ack receipt requires the receipt number, 
		// and that you inform the system of ID_SND_RECEIPT_ACKED and ID_SND_RECEIPT_LOSS
		serializeParameters->pro[0].sendReceipt=replicaManager->GetRakPeerInterface()->IncrementNextSendReceipt();
		serializeParameters->messageTimestamp=RakNet::GetTime();

		// Begin writing all variables to be sent UNRELIABLE_WITH_ACK_RECEIPT 
		variableDeltaSerializer.BeginUnreliableAckedSerialize(
			&serializationContext,
			serializeParameters->destinationConnection->GetRakNetGUID(),
			&serializeParameters->outputBitstream[0],
			serializeParameters->pro[0].sendReceipt
			);
		// Write each variable
		variableDeltaSerializer.SerializeVariable(&serializationContext, var1Unreliable);
		// Write each variable
		variableDeltaSerializer.SerializeVariable(&serializationContext, var2Unreliable);
		// Tell the system this is the last variable to be written
		variableDeltaSerializer.EndSerialize(&serializationContext);

		// All variables to be sent using a different mode go on different channels
		serializeParameters->pro[1].reliability=RELIABLE_ORDERED;

		// Same as above, all variables to be sent with a particular reliability are sent in a batch
		// We use BeginIdenticalSerialize instead of BeginSerialize because the reliable variables have the same values sent to all systems. This is memory-saving optimization
		variableDeltaSerializer.BeginIdenticalSerialize(
			&serializationContext,
			serializeParameters->whenLastSerialized==0,
			&serializeParameters->outputBitstream[1]
		);
		variableDeltaSerializer.SerializeVariable(&serializationContext, var3Reliable);
		variableDeltaSerializer.SerializeVariable(&serializationContext, var4Reliable);
		variableDeltaSerializer.EndSerialize(&serializationContext);

		// This return type makes is to ReplicaManager3 itself does not do a memory compare. 
		// we entirely control serialization ourselves here.
		// Use RM3SR_SERIALIZED_ALWAYS instead of RM3SR_SERIALIZED_ALWAYS_IDENTICALLY 
		// to support sending different data to different system, which is needed when using unreliable and dirty variable resends
		return RM3SR_SERIALIZED_ALWAYS;
	}

	void SampleReplica::Deserialize(RakNet::DeserializeParameters *deserializeParameters) {
		VariableDeltaSerializer::DeserializationContext deserializationContext;

		// Deserialization is written similar to serialization
		// Note that the Serialize() call above uses two different reliability types. This results in two separate Send calls
		// So Deserialize is potentially called twice from a single Serialize
		variableDeltaSerializer.BeginDeserialize(&deserializationContext, &deserializeParameters->serializationBitstream[0]);
		if (variableDeltaSerializer.DeserializeVariable(&deserializationContext, var1Unreliable))	{
			getPeer()->log(ELogType_Message, "%s deserialized: var1Unreliable changed to %i", getName().C_String(), var1Unreliable);
		}
		if (variableDeltaSerializer.DeserializeVariable(&deserializationContext, var2Unreliable))	{
			getPeer()->log(ELogType_Message, "%s deserialized: var2Unreliable changed to %i", getName().C_String(), var2Unreliable);
		}
		variableDeltaSerializer.EndDeserialize(&deserializationContext);

		variableDeltaSerializer.BeginDeserialize(&deserializationContext, &deserializeParameters->serializationBitstream[1]);
		if (variableDeltaSerializer.DeserializeVariable(&deserializationContext, var3Reliable))	{
			getPeer()->log(ELogType_Message, "%s deserialized: var3Reliable changed to %i", getName().C_String(), var3Reliable);
		}
		if (variableDeltaSerializer.DeserializeVariable(&deserializationContext, var4Reliable))	{
			getPeer()->log(ELogType_Message, "%s deserialized: var4Reliable changed to %i", getName().C_String(), var4Reliable);
		}
		variableDeltaSerializer.EndDeserialize(&deserializationContext);
	}


	void SampleReplica::OnPoppedConnection(RakNet::Connection_RM3 *droppedConnection)	{
		// Same as in SerializeDestruction(), no longer track this system
		variableDeltaSerializer.RemoveRemoteSystemVariableHistory(droppedConnection->GetRakNetGUID());
		getPeer()->log(ELogType_Message, "OnPoppedConnection for replica: %s", getName().C_String());
	}

	void SampleReplica::notifyReplicaOfMessageDeliveryStatus(RakNetGUID guid, uint32_t receiptId, bool messageArrived)	{
		// When using UNRELIABLE_WITH_ACK_RECEIPT, the system tracks which variables were updated with which sends
		// So it is then necessary to inform the system of messages arriving or lost
		// Lost messages will flag each variable sent in that update as dirty, meaning the next Serialize() call will resend them with the current values
		variableDeltaSerializer.OnMessageReceipt(guid,receiptId,messageArrived);
	}

	void SampleReplica::log()	{
		getPeer()->log(ELogType_Message, "replica: created(%s) %s", 
			getCreatedByReplicaManager() ? "local" : "remote",
			getName().C_String());
		getPeer()->log(ELogType_Message, "    var1Unreliable: %i", var1Unreliable);
		getPeer()->log(ELogType_Message, "    var2Unreliable: %i", var2Unreliable);
		getPeer()->log(ELogType_Message, "    var3Reliable:   %i", var3Reliable);
		getPeer()->log(ELogType_Message, "    var4Reliable:   %i", var4Reliable);
	}

	void SampleReplica::randomizeVariables(void)	{
		bool variableChanged(false);
		while(variableChanged == false)	{
			// note: randomize is not so useful in our context
			//       so we just increment by 1
			const bool randomize(false);
			if (randomMT()%2)	{
				variableChanged = true;
				var1Unreliable = randomize ? randomMT() : var1Unreliable + 1;
				getPeer()->log(ELogType_Message, "%s var1Unreliable changed to %i", getName().C_String(), var1Unreliable);
			}
			if (randomMT()%2)	{
				variableChanged = true;
				var2Unreliable = randomize ? randomMT() : var2Unreliable + 1;
				getPeer()->log(ELogType_Message, "%s var2Unreliable changed to %i", getName().C_String(), var2Unreliable);
			}
			if (randomMT()%2)	{
				variableChanged = true;
				var3Reliable = randomize ? randomMT() : var3Reliable + 1;
				getPeer()->log(ELogType_Message, "%s var3Reliable changed to %i", getName().C_String(), var3Reliable);
			}
			if (randomMT()%2)	{
				variableChanged = true;
				var4Reliable = randomize ? randomMT() : var4Reliable + 1;
				getPeer()->log(ELogType_Message, "%s var4Reliable changed to %i", getName().C_String(), var4Reliable);
			}
		}
	}
}

