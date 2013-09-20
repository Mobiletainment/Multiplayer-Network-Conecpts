//!---------------------------------------------------------------------------
//! \file "slCircularVector.h"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2012-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------


#pragma once
#ifndef __SLCIRCULARVECTOR_H__
#define __SLCIRCULARVECTOR_H__


#include "../compiler/slCompilerMacros.h"

namespace sltl
{
	//! a very simple non thread safe circular vector template
	template < typename T, size_t NUM_ELEMENTS = 100 >
	class CircularVector
	{ 
	public:

		typedef typename std::vector<T>::const_iterator const_iterator;
		typedef typename std::vector<T>::iterator iterator;

		CircularVector() : _idx(0),_maxElements(NUM_ELEMENTS)
		{
			_vec = std::vector<T>(_maxElements);
		}

		//! all data previously allocated will be destroyed
		void setCapacity(size_t value)	{
			_idx = 0;
			_maxElements = value;
			_vec = std::vector<T>(_maxElements);
		}

		void push_back(const T& elt)	{
			_vec[ _idx++ % _maxElements ] = elt;
		}

		size_t size() const { return std::min(_vec.size(), _idx); }

		const T& elementAt(size_t idx) const	{
			if(_idx < _vec.size() && idx < size())	{
				return _vec[idx];
			}
			else	{
				size_t elementIdx = (_idx + idx) % _maxElements;
				return _vec[elementIdx];
			}
		}

		size_t getMaxElements() const { return _maxElements;}

	private:
		size_t _maxElements;
		size_t _idx;
		std::vector<T> _vec;
	};
}



#endif // __SLCIRCULARVECTOR_H__
