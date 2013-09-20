#ifndef OPENSTEER_AABBOX_H
#define OPENSTEER_AABBOX_H

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

#include "OpenSteer/AbstractLocalSpace.h"
#include "OpenSteer/Color.h"

//-----------------------------------------------------------------------------
namespace OpenSteer {

	// an AABBox
	class AABBox{
	public:
		AABBox( void ) {};
		AABBox( const Vec3 &min, const Vec3& max );

		void initializeWithCenterAndExtent( const Vec3 &center, const Vec3& extent );

		bool insideX( const Vec3& p ) const;
		bool insideY( const Vec3& p ) const;
		bool insideZ( const Vec3& p ) const;

		bool inside( const Vec3& p ) const;
		bool inside( const AbstractLocalSpace& localSpace ) const;
		bool insideXZ( const Vec3& p ) const;
		bool insideXZ( const AbstractLocalSpace& localSpace ) const;
		bool insideXZWithRadius( const AbstractLocalSpace& localSpace ) const;

		bool insideXY( const Vec3& p ) const;
		bool insideXY( const AbstractLocalSpace& localSpace ) const;
		bool insideXYWithRadius( const AbstractLocalSpace& localSpace ) const;

		void draw( const Color& color ) const;
		void draw( void ) const;
	private:
		Vec3 _min;
		Vec3 _max;
		Vec3 _center;
		Vec3 _extent;
	};

} //! namespace OpenSteer    


//-----------------------------------------------------------------------------
#endif //! OPENSTEER_AABBOX_H
