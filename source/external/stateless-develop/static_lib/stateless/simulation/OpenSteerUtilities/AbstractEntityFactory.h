#ifndef __ABSTRACTENTITYFACTORY_H__
#define __ABSTRACTENTITYFACTORY_H__
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

// TODO OpenSteerCollector include
#include "OpenSteerUtilities/OpenSteerUTTypes.h"

//-----------------------------------------------------------------------------
namespace OpenSteer
{

	//-------------------------------------------------------------------------
	class AbstractEntityFactory
	{
	public:
		virtual ~AbstractEntityFactory() {}

		virtual AbstractEntity* accessMasterEntity( EntityClassId ) const ET_ABSTRACT;
		
		virtual AbstractEntity* getMasterEntity( void ) const ET_ABSTRACT;
		virtual void setMasterEntity( AbstractEntity* entity ) const ET_ABSTRACT;

		virtual AbstractEntity* createEntity( EntityClassId ) const ET_ABSTRACT;
		virtual AbstractVehicle* createVehicle( EntityClassId ) const ET_ABSTRACT;
		virtual AbstractVehicle* createVehicle( void ) const ET_ABSTRACT;

		virtual bool destroyEntity( AbstractEntity* entity ) const ET_ABSTRACT;
		virtual bool destroyVehicle( AbstractVehicle* vehicle ) const ET_ABSTRACT;
	protected:
	};


}


#endif // __ABSTRACTENTITYFACTORY_H__
