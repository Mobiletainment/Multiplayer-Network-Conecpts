//!---------------------------------------------------------------------------
//! \file "slApplicationAPI.h"
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
#ifndef __SLAPPLICATIONAPI_H__
#define __SLAPPLICATIONAPI_H__

//<license>
//-----------------------------------------------------------------------------
// Copyright (c) 2013, Cyrus Preuss
// All rights reserved.
//-----------------------------------------------------------------------------
//</license>

//
// The following block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the Foundation_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// Foundation_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
//
#if (defined(_WIN32) || defined(_WIN32_WCE)) && defined(NL_DLL)
#if defined(slApplication_EXPORTS)
#define slApplication_API __declspec(dllexport)
#else
#define slApplication_API __declspec(dllimport)	
#endif
#endif


#if !defined(slApplication_API)
#if !defined(NL_NO_GCC_API_ATTRIBUTE) && defined (__GNUC__) && (__GNUC__ >= 4)
#define slApplication_API __attribute__ ((visibility ("default")))
#else
#define slApplication_API
#endif
#endif




//-----------------------------------------------------------------------------

#endif  // __SLFRAMEWORKAPI_H__
