//!---------------------------------------------------------------------------
//! \file "slAbstractNode.cpp"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2012-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------


#include "stdafx.h"
#include "slAbstractNode.h"


namespace sl	{

	AbstractNode::AbstractNode() {

	}
	AbstractNode::~AbstractNode() {

	}


	AbstractNodeArray::AbstractNodeArray()
	{

	}

	AbstractNodeArray::~AbstractNodeArray()
	{

	}

	void AbstractNodeArray::add(AbstractNode* ctrl)	{
		TAbstractNodeArray::const_iterator ctrlIter(std::find (begin(), end(), ctrl));
		if(ctrlIter == end())	{
			push_back(ctrl);
		}
	}

	void AbstractNodeArray::remove(AbstractNode* ctrl)	{
		TAbstractNodeArray::const_iterator ctrlIter(std::find (begin(), end(), ctrl));
		if(ctrlIter != end())	{
			erase(ctrlIter);
		}
	}

	SLSize AbstractNodeArray::nodeCount() const
	{
		return size();
	}

	AbstractNode* AbstractNodeArray::nodeAt( SLSize idx )
	{
		if(idx < size())	{
			return (*this)[idx];
		}
		return nullptr;
	}

}
