//!---------------------------------------------------------------------------
//! \file "slAbstractNode.h"
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
#ifndef __SLABSTRACTNODE_H__
#define __SLABSTRACTNODE_H__



namespace sl	{

	class slCocosUtil_API AbstractNode : public CCNode
	{
	protected:
		AbstractNode();
		virtual ~AbstractNode();
	};


	template class slCocosUtil_API std::allocator<AbstractNode*>;
	typedef std::vector<AbstractNode*, std::allocator<AbstractNode*>> TAbstractNodeArray;

	class slCocosUtil_API AbstractNodeArray : private TAbstractNodeArray	{
	public:
		AbstractNodeArray();
		virtual ~AbstractNodeArray();

		void add(AbstractNode* ctrl);

		void remove(AbstractNode* ctrl);

		SLSize nodeCount() const;

		AbstractNode* nodeAt(SLSize idx);
	};


}


#endif
