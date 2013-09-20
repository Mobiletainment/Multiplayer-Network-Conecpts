#ifndef __INSTANCECOUNT_H__
#define __INSTANCECOUNT_H__

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

//-----------------------------------------------------------------------------
namespace EduNet	{

//-----------------------------------------------------------------------------
	class InstanceCount
	{
	public:
		InstanceCount(): m_nConstructed(0), m_nDestructed(0), m_nOnCreateCalls(0), m_nAssigned(0)
		{
		}

		virtual ~InstanceCount() {}

		size_t Constructor()
		{
			return ++this->m_nConstructed;
		}

		size_t Destructor()
		{
			return ++this->m_nDestructed;
		}

		size_t Assign()
		{
			return ++this->m_nAssigned;
		}

		size_t OnCreate()
		{
			return ++this->m_nOnCreateCalls;
		}

		size_t GetConstructedCount() const
		{
			return this->m_nConstructed;
		}

		size_t GetDestructedCount() const
		{
			return this->m_nDestructed;
		}

		size_t GetAssignCount() const
		{
			return this->m_nAssigned;
		}

		size_t GetInstanceCount() const
		{
			if (this->m_nDestructed > this->m_nConstructed)
			{
				return 0;
			}
			return this->m_nConstructed - this->m_nDestructed;
		}

		void Reset()
		{
			m_nConstructed = m_nDestructed = m_nAssigned = m_nOnCreateCalls = 0;
		}

	private:
		size_t m_nConstructed;
		size_t m_nDestructed;
		size_t m_nAssigned;
		size_t m_nOnCreateCalls;
	};


} // namespace EduNet

#endif // __INSTANCECOUNT_H__