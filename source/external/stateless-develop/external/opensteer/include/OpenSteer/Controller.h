#pragma once
#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

//!---------------------------------------------------------------------------
//! \file "Controller.h"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2012-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------

#include "OpenSteer/AbstractController.h"
#include "OpenSteer/Entity.h"
#include "OpenSteer/Vec3.h"

//-----------------------------------------------------------------------------
namespace OpenSteer
{

	// contoller mixin templates
	typedef EntityVisibilityMixin<AbstractController> TAbstractController_000;
	typedef EntityParentMixin<TAbstractController_000> TAbstractController_00;
	typedef EntityMixin<TAbstractController_00> TAbstractController_0;

	typedef AbstractUpdatedMixin<TAbstractController_0> TAbstractController;

	//-------------------------------------------------------------------------
	class Controller : public TAbstractController
	{
		OS_DECLARE_BASE( TAbstractController )
	public:
		Controller();
		virtual ~Controller();
		OS_IMPLEMENT_CLASSNAME( Controller )

		void reset( void );

		virtual float getActionValue( EControllerAction action ) const;
		virtual void setActionValue( EControllerAction action, float fValue );

		virtual void resetValues(  );

		//---------------------------------------------------------------------
		// AbstractUpdated interface
		virtual void update( const osScalar currentTime, const osScalar elapsedTime );

	protected:
		void setAxisValue( size_t axis, float fValue );
		void setAxisMapping( size_t axis, EControllerAction action, size_t uiIdx = 0 );
		bool hasAxisMapping( size_t axis ) const;
	private:
		int _axisMapping[EControllerAction_Count][2];
		enum
		{
			EAxis_Count = 1024
		};
		float _actionValue[EControllerAction_Count];
		float _axisValue[EAxis_Count];
		bool _axisMapped[EAxis_Count];
	};


}

#endif //  __CONTROLLER_H__