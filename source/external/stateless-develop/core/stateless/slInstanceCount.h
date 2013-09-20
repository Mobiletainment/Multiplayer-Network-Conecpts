#pragma once
#ifndef __SLINSTANCECOUNT_H__
#define __SLINSTANCECOUNT_H__

//!---------------------------------------------------------------------------
//! \file "slInstanceCount.h"
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


	class InstanceCount
	{
	public:
		InstanceCount()
			: _constructed(0)
			,_destructed(0)
			,_onCreateCalls(0)
			,_assigned(0)	
		{
		}

		virtual ~InstanceCount() {

		}

		size_t constructor()	{
			return ++_constructed;
		}

		size_t destructor()	{
			return ++_destructed;
		}

		size_t assign()	{
			return ++_assigned;
		}

		size_t onCreate()	{
			return ++_onCreateCalls;
		}

		size_t getConstructedCount() const	{
			return _constructed;
		}

		size_t getDestructedCount() const	{
			return _destructed;
		}

		size_t getAssignCount() const	{
			return _assigned;
		}

		size_t getInstanceCount() const	{
			if (_destructed > _constructed)
			{
				return 0;
			}
			return _constructed - _destructed;
		}

		void reset()	{
			_constructed = _destructed = _assigned = _onCreateCalls = 0;
		}

		void resetIfAllInstancesAreDestroyed()	{
			if(getInstanceCount() == 0)	{
				reset();
			}
		}

	private:
		size_t _constructed;
		size_t _destructed;
		size_t _assigned;
		size_t _onCreateCalls;
	};
}

#endif  // __SLINSTANCECOUNT_H__
