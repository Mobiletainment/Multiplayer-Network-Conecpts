#ifndef __INSTANCETRACKER_H__
#define __INSTANCETRACKER_H__

//-----------------------------------------------------------------------------
//! Copyright (c) 2011, Jan Fietz, Cyrus Preuss
//! All rights reserved.
//! 
//! Redistribution and use in source and binary forms, with or without modification, 
//! are permitted provided that the following conditions are met:
//! 
//! * Redistributions of source code must retain the above copyright notice, 
//!   this list of conditions and the following disclaimer.
//! * Redistributions in binary form must reproduce the above copyright notice, 
//!   this list of conditions and the following disclaimer in the documentation 
//!   and/or other materials provided with the distribution.
//! * Neither the name of EduNetGames nor the names of its contributors
//!   may be used to endorse or promote products derived from this software
//!   without specific prior written permission.
//! 
//! THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//! ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
//! WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
//! IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
//! INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
//! (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
//! LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON 
//! ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
//! (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
//! EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------


// for size_t
#include <cstring>

//-----------------------------------------------------------------------------
namespace OpenSteer	{

//-----------------------------------------------------------------------------
	template <class TypeId>
	class TInstanceTracker
	{
	public:
		TInstanceTracker()
			:_constructed(TypeId(0)) 
			,_destructed(TypeId(0))
			,_onCreateCalls(TypeId(0)) 
			,_assigned(TypeId(0))
		{
		}

		virtual ~TInstanceTracker() {}

		TypeId constructor()	{
			return ++_constructed;
		}

		TypeId destructor()	{
			return ++_destructed;
		}

		TypeId assign()	{
			return ++_assigned;
		}

		TypeId onCreate()	{
			return ++_onCreateCalls;
		}

		TypeId getConstructedCount() const	{
			return _constructed;
		}

		TypeId getDestructedCount() const	{
			return _destructed;
		}

		TypeId getAssignCount() const	{
			return _assigned;
		}

		TypeId getInstanceCount() const	{
			if (_destructed > _constructed)
			{
				return TypeId(0);
			}
			return _constructed - _destructed;
		}

		void reset()	{
			_constructed = _destructed = _assigned = _onCreateCalls = TypeId(0);
		}
		typedef TypeId Id;

	private:
		TypeId _constructed;
		TypeId _destructed;
		TypeId _assigned;
		TypeId _onCreateCalls;
	};

	typedef TInstanceTracker<size_t> InstanceTracker;


}

#endif //! __INSTANCECOUNT_H__
