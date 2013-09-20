//!---------------------------------------------------------------------------
//! \file "slFlagSet.h"
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
#ifndef __SLFLAGSET_H__
#define __SLFLAGSET_H__


#include "../compiler/slCompilerMacros.h"

namespace sltl
{
	//-----------------------------------------------------------------------------
	// a generic flag set
	template <class ENUM, typename Tint=unsigned long> 
	class FlagSet
	{
	public:
		typedef Tint value_type;

		FlagSet() throw() :
			_flags(0)
		{ }

		explicit FlagSet( Tint val ) throw() :
			_flags(val)
		{ }

		virtual ~FlagSet()
		{ }

		void addFlag( __in ENUM flag ) throw();
		bool hasAnyFlag( void ) const throw();
		bool hasFlag( __in ENUM flag ) const throw();
		bool hasAnyOfTheFlags( __in Tint flags) const throw();
		void removeFlag( __in ENUM flag ) throw();
		void toggleFlag( __in ENUM flag ) throw();

		void setFlags(__in  value_type nFlags ) throw() { _flags = nFlags; };
		value_type getFlags( void ) const throw() { return _flags; }

		bool operator==(__in const FlagSet& other) const throw()
		{
			return _flags==other._flags;
		};
		bool operator!=(__in const FlagSet& other) const throw()
		{
			return _flags!=other._flags;
		};

	private:
		value_type _flags;
	};

	//-----------------------------------------------------------------------------
	template <class ENUM, typename Type> 
	void FlagSet< ENUM, Type >::addFlag( ENUM flag ) throw()
	{
		_flags |= flag;
	}

	//-----------------------------------------------------------------------------
	template <class ENUM, typename Type>
	bool FlagSet< ENUM, Type >::hasAnyFlag( void ) const throw()
	{
		return ( 0 != _flags );
	}

	//-----------------------------------------------------------------------------
	template <class ENUM, typename Type>
	bool FlagSet< ENUM, Type >::hasFlag( ENUM flag ) const throw()
	{
		return ( flag == ( _flags & flag ) );
	}

	//-----------------------------------------------------------------------------
	template <class ENUM, typename Type>
	bool FlagSet< ENUM, Type >::hasAnyOfTheFlags( __in Type flags) const throw()
	{
		return ( 0 != (_flags & flags));
	}

	//-----------------------------------------------------------------------------
	template <class ENUM, typename Type>
	void FlagSet< ENUM, Type >::removeFlag( ENUM flag ) throw()
	{
		_flags &= ~flag;
	}

	//-----------------------------------------------------------------------------
	template <class ENUM, typename Type>
	void FlagSet< ENUM, Type >::toggleFlag( ENUM flag ) throw()
	{
		_flags ^= flag;
	}

	template <class ENUM>
	class AbstractFlagset	{
	public:
		virtual void addFlag( __in ENUM flag ) SL_ABSTRACT;
		virtual void removeFlag( __in ENUM flag ) SL_ABSTRACT;
		virtual bool hasFlag( __in ENUM flag ) const SL_ABSTRACT;
		virtual void toggleFlag( __in ENUM flag ) SL_ABSTRACT;
	};

	//-----------------------------------------------------------------------------
	//! \brief mixin template to add flag functionalities to a certain class
	template <class Super, class ENUM, typename Tint=unsigned long>
	class FlagSetMixin : public Super	{
	public:

		virtual void addFlag( __in ENUM flag ) throw() { return _flags.addFlag(flag); };
		virtual void removeFlag( __in ENUM flag ) throw() { return _flags.removeFlag(flag); };
		virtual bool hasFlag( __in ENUM flag ) const throw() { return _flags.hasFlag(flag); };
		virtual void toggleFlag( __in ENUM flag ) throw() { return _flags.toggleFlag(flag); };

		void setFlags( __in typename FlagSet<ENUM, Tint>::value_type nFlags ) throw() { _flags.setFlags(nFlags); };
		typename FlagSet<ENUM, Tint>::value_type getFlags() const throw() { return _flags.getFlags(); }

	private:
		FlagSet<ENUM, Tint> _flags;
	};
}



#endif // __SLFLAGSET_H__
