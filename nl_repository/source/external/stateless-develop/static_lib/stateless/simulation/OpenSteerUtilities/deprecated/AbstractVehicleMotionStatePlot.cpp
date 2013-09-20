#include "stdafx.h"

#if 0
//-----------------------------------------------------------------------------
// Copyright (c) 2011, Jan Fietz, Cyrus Preuss
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification, 
// are permitted provided that the following conditions are met:
// 
// * Redistributions of source code must retain the above copyright notice, 
//   this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice, 
//   this list of conditions and the following disclaimer in the documentation 
//   and/or other materials provided with the distribution.
// * Neither the name of EduNetGames nor the names of its contributors
//   may be used to endorse or promote products derived from this software
//   without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON 
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#include "AbstractVehicleMotionStatePlot.h"
#include "SimplePhysicsVehicle.h"

//-----------------------------------------------------------------------------
namespace OpenSteer {

	//-------------------------------------------------------------------------
	AbstractVehicleMotionStatePlot::AbstractVehicleMotionStatePlot():
		m_pkClientVehicle( NULL ),
		m_pkServerVehicle( NULL )
	{
		const size_t uiNumRecords = 50;
		Color kColor0(0.0f, 1.0f, 0.0f);
		Color kColor1(1.0f, 0.0f, 0.0f);

		Color kColor00(0.25f, 1.0f, 0.0f);
		Color kColor10(1.0f, 0.25f, 0.0f);

		this->m_kLinearVelocity.accessValues(0).
			setName( "LinearVel" ).
			setColor( kColor0.r(), kColor0.g(), kColor0.b(), kColor0.a() ).
			setMaxRecords( uiNumRecords );
		this->m_kAngularVelocity.accessValues(0).
			setName( "AngularVel" ).
			setColor( kColor0.r(), kColor0.g(), kColor0.b(), kColor0.a() ).
			setMaxRecords( uiNumRecords );
		this->m_kSteeringForce.accessValues(0).
			setName( "SteeringForce" ).
			setColor( kColor0.r(), kColor0.g(), kColor0.b(), kColor0.a() ).
			setMaxRecords( uiNumRecords );

		this->m_kLinearVelocity.accessValues(1).
			setName( "LinearVel" ).
			setColor( kColor1.r(), kColor1.g(), kColor1.b(), kColor1.a() ).
			setMaxRecords( uiNumRecords );
		this->m_kAngularVelocity.accessValues(1).
			setName( "AngularVel" ).
			setColor( kColor1.r(), kColor1.g(), kColor1.b(), kColor1.a() ).
			setMaxRecords( uiNumRecords );
		this->m_kSteeringForce.accessValues(1).
			setName( "SteeringForce" ).
			setColor( kColor1.r(), kColor1.g(), kColor1.b(), kColor1.a() ).
			setMaxRecords( uiNumRecords );

		this->m_kLinearVelocity.accessValues(2).
			setName( NULL ).
			setColor( kColor00.r(), kColor00.g(), kColor00.b(), kColor00.a() ).
			setMaxRecords( uiNumRecords ).setGraphType( Profile::EGraphType_Dots );
		this->m_kAngularVelocity.accessValues(2).
			setName( NULL ).
			setColor( kColor00.r(), kColor00.g(), kColor00.b(), kColor00.a() ).
			setMaxRecords( uiNumRecords ).setGraphType( Profile::EGraphType_Dots );
		this->m_kSteeringForce.accessValues(2).
			setName( NULL ).
			setColor( kColor00.r(), kColor00.g(), kColor00.b(), kColor00.a() ).
			setMaxRecords( uiNumRecords ).setGraphType( Profile::EGraphType_Dots );

		this->m_kLinearVelocity.accessValues(3).
			setName( NULL ).
			setColor( kColor10.r(), kColor10.g(), kColor10.b(), kColor10.a() ).
			setMaxRecords( uiNumRecords ).setGraphType( Profile::EGraphType_Dots );
		this->m_kAngularVelocity.accessValues(3).
			setName( NULL ).
			setColor( kColor10.r(), kColor10.g(), kColor10.b(), kColor10.a() ).
			setMaxRecords( uiNumRecords ).setGraphType( Profile::EGraphType_Dots );
		this->m_kSteeringForce.accessValues(3).
			setName( NULL ).
			setColor( kColor10.r(), kColor10.g(), kColor10.b(), kColor10.a() ).
			setMaxRecords( uiNumRecords ).setGraphType( Profile::EGraphType_Dots );

	}

	//-------------------------------------------------------------------------
	AbstractVehicleMotionStatePlot::~AbstractVehicleMotionStatePlot()
	{
	}

	//-------------------------------------------------------------------------
	void AbstractVehicleMotionStatePlot::recordUpdate( AbstractVehicle* pkVehicle, const float currentTime, const float elapsedTime )
	{
		if( NULL == pkVehicle )
		{
			return;
		}
		size_t uiRecordIndex = pkVehicle->isRemoteObject() ? 1 : 0;
		if( pkVehicle->isRemoteObject() )
		{
			this->m_pkClientVehicle = pkVehicle;
		}
		else
		{
			this->m_pkServerVehicle = pkVehicle;
		}
		// update motion state plot
		SimplePhysicsVehicle* pkPhysicsVehicle = dynamic_cast<SimplePhysicsVehicle*>(pkVehicle);
		if( pkPhysicsVehicle != NULL )
		{
			{
				const OpenSteer::EulerVehicleUpdate& kEulerUpdate = pkPhysicsVehicle->getEulerUpdate();
				const PhysicsMotionState& kState = kEulerUpdate.getMotionState();

				Profile::GraphValues& kLinearVelocityValues = 
					this->m_kLinearVelocity.accessValues(uiRecordIndex);
				kLinearVelocityValues.addValue( currentTime, kState.m_fLinearVelocity );
//				kLinearVelocityValues.addValue( currentTime, pkVehicle->linearVelocity().length() );

				Profile::GraphValues& kAngularVelocityValues = 
					this->m_kAngularVelocity.accessValues(uiRecordIndex);
				kAngularVelocityValues.addValue( currentTime, kState.m_fAngularVelocity );
//				kAngularVelocityValues.addValue( currentTime, pkVehicle->angularVelocity().length() );

				Profile::GraphValues& kSteeringForceValues = 
					this->m_kSteeringForce.accessValues(uiRecordIndex);
				kSteeringForceValues.addValue( currentTime, kEulerUpdate.getForce().length() );
			}

			// now add some fake values to the net send/receive graph
			uiRecordIndex = pkVehicle->isRemoteObject() ? 3 : 2;
			{
				Profile::GraphValues& kLinearVelocityValues = 
					this->m_kLinearVelocity.accessValues(uiRecordIndex);
				kLinearVelocityValues.addValue( currentTime, 0.0f );

				Profile::GraphValues& kAngularVelocityValues = 
					this->m_kAngularVelocity.accessValues(uiRecordIndex);
				kAngularVelocityValues.addValue( currentTime, 0.0f );

				Profile::GraphValues& kSteeringForceValues = 
					this->m_kSteeringForce.accessValues(uiRecordIndex);
				kSteeringForceValues.addValue( currentTime, 0.0f );
			}
		}
	}

	//-------------------------------------------------------------------------
	void AbstractVehicleMotionStatePlot::recordNetUpdate( AbstractVehicle* pkVehicle, const float currentTime, const float elapsedTime )
	{
		if( NULL == pkVehicle )
		{
			return;
		}

		if( ( this->m_pkClientVehicle != pkVehicle ) &&
			( this->m_pkServerVehicle != pkVehicle ) )
		{
			return;
		}

		size_t uiRecordIndex = pkVehicle->isRemoteObject() ? 3 : 2;
		// update motion state plot
		SimplePhysicsVehicle* pkPhysicsVehicle = dynamic_cast<SimplePhysicsVehicle*>(pkVehicle);
		if( pkPhysicsVehicle != NULL )
		{
			const OpenSteer::EulerVehicleUpdate& kEulerUpdate = pkPhysicsVehicle->getEulerUpdate();
			//			const PhysicsMotionState& kState = kEulerUpdate.getMotionState();

			Profile::GraphValues& kLinearVelocityValues = 
				this->m_kLinearVelocity.accessValues(uiRecordIndex);
			//			kLinearVelocityValues.addValue( currentTime, kState.m_fLinearVelocity );
			kLinearVelocityValues.addValue( currentTime, pkVehicle->linearVelocity().length() );

			Profile::GraphValues& kAngularVelocityValues = 
				this->m_kAngularVelocity.accessValues(uiRecordIndex);
			//			kAngularVelocityValues.addValue( currentTime, kState.m_fAngularVelocity );
			kAngularVelocityValues.addValue( currentTime, pkVehicle->angularVelocity().length() );
			Profile::GraphValues& kSteeringForceValues = 
				this->m_kSteeringForce.accessValues(uiRecordIndex);
			kSteeringForceValues.addValue( currentTime, kEulerUpdate.getForce().length() );
		}
	}

	//-------------------------------------------------------------------------
	void AbstractVehicleMotionStatePlot::draw( float currentTime ) const
	{
		// draw only once
		if( currentTime == this->m_currentTime )
		{
			return;
		}
		this->m_currentTime = currentTime;
		// draw motion state plot
		const float fGraphStart = 50;
		const float fGraphHeight = 150;
		const float fGraphWidth = 600;
		Profile::GraphPlot kPlot;
		Profile::TGraphPointerArray kGraphArray;
		kGraphArray.push_back( &this->m_kLinearVelocity );
		kGraphArray.push_back( &this->m_kAngularVelocity );
		kGraphArray.push_back( &this->m_kSteeringForce );
		kPlot.draw( kGraphArray, 50, fGraphStart, fGraphWidth, fGraphHeight * kGraphArray.size() );
	}



} // namespace OpenSteer


#endif

