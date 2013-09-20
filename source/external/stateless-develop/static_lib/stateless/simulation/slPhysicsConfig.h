#pragma once
#ifndef __SLPHYSICSCONFIG_H__
#define __SLPHYSICSCONFIG_H__

//!---------------------------------------------------------------------------
//! \file "slPhysicsConfig.h"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2012-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------




namespace sl	{

	class AbstractPhysicsConfig	{
	public:
		SL_SYNTHESIZE(SLTimeInterval, _frameTime, FrameTime)
		SL_SYNTHESIZE(SLDouble, _frequency, Frequency)
	protected:
		AbstractPhysicsConfig();
	};


	class PhysicsConfig	: public AbstractPhysicsConfig	{
		SL_DECLARE_BASE(AbstractPhysicsConfig)
	public:
		static PhysicsConfig& sharedInstance();

		virtual void setFrameTime(SLTimeInterval frameTime) SL_OVERRIDE;
		virtual void setFrequency(SLDouble frequency) SL_OVERRIDE;
	private:
		PhysicsConfig();
	};

}

#endif // __SLPHYSICSCONFIG_H__
