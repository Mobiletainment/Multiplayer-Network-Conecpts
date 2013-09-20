#pragma once
#ifndef __SLCCSMARTPOINTER_H__
#define __SLCCSMARTPOINTER_H__
//!---------------------------------------------------------------------------
//! \file "slCCSmartPointer.h"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2012-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------


//
//  slCCSmartPtr.h
//  slCocosUtil
//
//  Created by Cyrus Preuss on 4/22/13.
//  Copyright (c) 2013 Cyrus Preuss. All rights reserved.
//



#if 0
// TODO: later
#  include "slAtomicOperations.h"
#endif

//#include "../compiler/slCompilerMacros.h"


namespace sl {
	void* /*__cdecl*/ AtomicExchangePointer(
											_Inout_  void ** target,
											_In_opt_ void * value
											);

#if (CC_TARGET_PLATFORM  == CC_PLATFORM_IOS) //
	inline
	void* /*__cdecl*/ AtomicExchangePointer(
											_Inout_  void ** target,
											_In_opt_ void * value
											)	{
		void * prevValue;
		(prevValue = *target, *(target) = value, __sync_synchronize());
		return prevValue;
	}
#endif
#if (CC_TARGET_PLATFORM  == CC_PLATFORM_WIN32) || defined(WIN32)
	inline
	void* /*__cdecl*/ AtomicExchangePointer(
		_Inout_  void ** target,
		_In_opt_ void * value
		)	{

			/*
			LONG __cdecl InterlockedExchange(
				_Inout_  LONG volatile *Target,
				_In_     LONG Value
				);
			*/
			void * prevValue = static_cast<void*>(InterlockedExchangePointer(target, value));
			return prevValue;
	}
#endif

}

//-----------------------------------------------------------------------------
namespace sltl
{
    /** @brief smartpointer template
	 @code{.cpp}
	 #include "slCCSmartPointer.h"
	 using namespace sltl;
	 typedef TCCSmartPointer<CCObject> CCObjectPtr;

	 class AAA : public CCObject
	 {
	 protected:
	 virtual ~AAA()
	 {
	 }
	 };
	 
	 ...
	 {
	 CCObjectPtr o(new AAA);
	 o->release();
	 }
	 {
	 CCObjectPtr o(new AAA);
	 o->autorelease();
	 }
	 ...
	 @endcode
	 */
	template <class Type>
	class TCCSmartPointer
	{
	public:
		TCCSmartPointer() throw();
		TCCSmartPointer(__in_opt Type* p) throw();
		TCCSmartPointer(__in const TCCSmartPointer& sp) throw();
		virtual ~TCCSmartPointer();

		static TCCSmartPointer createInstance()	{
			Type* p(Type::create());
			TCCSmartPointer sptr(p);
			return sptr;
		}

		TCCSmartPointer& operator=(__in const TCCSmartPointer& sp) throw();
		TCCSmartPointer& operator=(__in_opt Type* p) throw();

		bool operator==(__in_opt Type* p) const throw();
		bool operator!=(__in_opt Type* p) const throw();
		bool operator==(__in const TCCSmartPointer& sp) const throw();
		bool operator!=(__in const TCCSmartPointer& sp) const throw();
		bool isNull() const throw();

		operator Type*() const throw();
		Type* operator->() const throw();

		Type* instance( void ) const throw()
		{
			return accessInstance();
		}

	protected:
		void setInstance( Type* p ) throw();
		Type* getInstance( void ) const throw()
		{
			return _instance;
		}
		// The assert on accessInstance usually indicates a bug.  If this is really
		// what is needed, however, take the address of the p member explicitly.
		Type* accessInstance( void ) const throw()
		{
			CCAssert( nullptr != _instance, "instance should not be nullptr" );
			return _instance;
		}
		Type* _instance;
	};

	//-----------------------------------------------------------------------------
	template<class Type>
	void TCCSmartPointer<Type>::setInstance( Type* p ) throw()
	{
		if(_instance != p)	{
			if( p != nullptr )	{
				p->retain();
			}
			Type* const prevInstance(static_cast<Type*>(sl::AtomicExchangePointer((void **)(&_instance), p)));
			if(prevInstance  != nullptr)	{
				prevInstance->release();
			}
		}
	}

	//-----------------------------------------------------------------------------
	template<class Type>
	TCCSmartPointer<Type>::TCCSmartPointer() throw():_instance(nullptr)
	{
	}

	//-----------------------------------------------------------------------------
	template<class Type>
	TCCSmartPointer<Type>::TCCSmartPointer(Type* p) throw():_instance(nullptr)
	{
		setInstance( p );
	}

	//-----------------------------------------------------------------------------
	template<class Type>
	TCCSmartPointer<Type>::TCCSmartPointer(const TCCSmartPointer& sp) throw():_instance(nullptr)
	{
		setInstance( sp._instance );
	}

	//-----------------------------------------------------------------------------
	template<class Type>
	TCCSmartPointer<Type>::~TCCSmartPointer()
	{
		setInstance( nullptr );
	}

	//-----------------------------------------------------------------------------
	template<class Type>
	TCCSmartPointer<Type>& TCCSmartPointer<Type>::operator=(const TCCSmartPointer<Type>& sp) throw()
	{
		setInstance( sp._instance );
		return *this;
	}

	//-----------------------------------------------------------------------------
	template<class Type>
	TCCSmartPointer<Type>& TCCSmartPointer<Type>::operator=(Type* p) throw()
	{
		setInstance( p );
		return *this;
	}

	//-----------------------------------------------------------------------------
	template<class Type>
	TCCSmartPointer<Type>::operator Type*() const throw()
	{
		return _instance;
	}

	//-----------------------------------------------------------------------------
	template<class Type>
	Type* TCCSmartPointer<Type>::operator->() const throw()
	{
		CCAssert( nullptr != _instance, "instance should not be nullptr" );
		return _instance;
	}

	//-----------------------------------------------------------------------------
	template<class T>
	bool TCCSmartPointer<T>::operator==(T* p) const throw()
	{
		return _instance == p;
	}

	//-----------------------------------------------------------------------------
	template<class T>
	bool TCCSmartPointer<T>::operator==(const TCCSmartPointer<T>& sp) const throw()
	{
		return _instance == sp._instance;
	}

	//-----------------------------------------------------------------------------
	template<class T>
	bool TCCSmartPointer<T>::operator!=(T* p) const throw()
	{
		return _instance != p;
	}

	//-----------------------------------------------------------------------------
	template<class T>
	bool TCCSmartPointer<T>::operator!=(const TCCSmartPointer<T>& sp) const throw()
	{
		return _instance != sp._instance;
	}

	//-----------------------------------------------------------------------------
	template<class Type>
	bool TCCSmartPointer<Type>::isNull() const throw()
	{
		return (_instance == nullptr);
	}
}


#endif /* defined(__SLCCSMARTPOINTER_H__) */
