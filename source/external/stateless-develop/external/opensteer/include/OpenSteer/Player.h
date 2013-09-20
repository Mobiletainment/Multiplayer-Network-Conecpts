#pragma once
#ifndef __OPENSTEERPLAYER_H__
#define __OPENSTEERPLAYER_H__

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

#include "OpenSteer/AbstractPlayer.h"
#include "OpenSteer/Entity.h"
#include "OpenSteer/Vec3.h"

#include "OpenSteer/Controller.h"

//-----------------------------------------------------------------------------
namespace OpenSteer
{
	// player mixin templates
	typedef EntityVisibilityMixin<AbstractPlayer> TAbstractPlayer_000;
	typedef EntityParentMixin<TAbstractPlayer_000> TAbstractPlayer_00;
	typedef EntityMixin<TAbstractPlayer_00> TAbstractPlayer_0;

	typedef AbstractUpdatedMixin<TAbstractPlayer_0> TAbstractPlayer;

	//-------------------------------------------------------------------------
	class Player : public TAbstractPlayer
	{
		OS_DECLARE_BASE( TAbstractPlayer )
	public:
		Player( bool isLocalPlayer = false );
		virtual ~Player();

		OS_IMPLEMENT_CLASSNAME( Player )

		//---------------------------------------------------------------------
		// AbstractPlayer interface
		virtual void setController( AbstractController* controller)	{
			_controller = controller;
		}

		virtual AbstractController* accessController( void ) const	{
			return _controller;
		}

		virtual AbstractController const* const getController( void ) const	{
			return _controller;
		}

		virtual bool isPlaying( void ) const	{
			return (getControlledEntity() != nullptr);
		}

		virtual bool isLocalPlayer( void ) const	{
			return _isLocalPlayer;
		}

		//---------------------------------------------------------------------
		// AbstractUpdated interface
		virtual void update( const osScalar currentTime, const osScalar elapsedTime );

	private:
		Controller _defaultController;

		AbstractController* _controller;
		bool _isLocalPlayer;
	};

}

#endif //  __OPENSTEERPLAYER_H__