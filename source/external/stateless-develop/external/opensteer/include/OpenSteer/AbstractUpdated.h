#ifndef __ABSTRACTUPDATED_H__
#define __ABSTRACTUPDATED_H__

/*
 *  AbstractUpdated.h
 *  OpenSteer
 *
 *  Created by Cyrus Preuss on 11/9/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "OpenSteer/OpenSteerMacros.h"
#include "OpenSteer/OpenSteerTypes.h"

//-----------------------------------------------------------------------------
namespace OpenSteer {
	
	//-------------------------------------------------------------------------
	class AbstractUpdated {
    public:
        virtual ~AbstractUpdated( void ) { /* Nothing to do. */ }
		virtual void updateCustom( AbstractUpdated* parent, const osScalar /*currentTime*/, const osScalar /*elapsedTime*/ ) OS_ABSTRACT;
		virtual void update( const osScalar /*currentTime*/, const osScalar /*elapsedTime*/ ) OS_ABSTRACT;
		virtual void setCustomUpdated( AbstractUpdated* ) OS_ABSTRACT;
		virtual AbstractUpdated* getCustomUpdated( void ) const OS_ABSTRACT; 
		virtual bool isEnabled( void ) const OS_ABSTRACT; 
		virtual void setEnabled( bool bEnabled ) OS_ABSTRACT; 
	};

	//-------------------------------------------------------------------------
	template <class Super>
	class AbstractUpdatedMixin : public Super {
	public:
		AbstractUpdatedMixin(  ): _customUpdated(0),_enabled(true)
		{
		}
		virtual ~AbstractUpdatedMixin(){}

		//-------------------------------------------------------------------
		// interface AbstractUpdated
		//---------------------------------------------------------------------
		virtual void updateCustom( AbstractUpdated* /*parent*/, const osScalar /*currentTime*/, const osScalar /*elapsedTime*/ )
		{
			// nothing to do here
			return;
		}
		virtual void update( const osScalar currentTime, const osScalar elapsedTime )
		{
			if( _customUpdated != nullptr )
			{
				// in case the custom updater decides to call the base class
				// prevent infinite recursion, store the custom updater locally
				// and restore it once done with the update
				AbstractUpdated* customUpdated(_customUpdated);
				_customUpdated = nullptr;
				customUpdated->updateCustom( this, currentTime, elapsedTime );
				_customUpdated = customUpdated;
			}
		}

		virtual void setCustomUpdated( AbstractUpdated* updated )
		{
			_customUpdated = updated;
		}

		virtual AbstractUpdated* getCustomUpdated( void ) const
		{
			return _customUpdated;
		}

		virtual bool isEnabled( void ) const { return _enabled; }; 
		virtual void setEnabled( bool bEnabled ){ _enabled = bEnabled; }; 

	private:
		AbstractUpdated* _customUpdated;
		bool _enabled;

	};

}

#endif  //! __ABSTRACTUPDATED_H__
