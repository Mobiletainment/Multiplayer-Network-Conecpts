#pragma once
#ifndef __SLFRAMEWORKAPI_H__
#define __SLFRAMEWORKAPI_H__

//!---------------------------------------------------------------------------
//! \file "slFrameworkAPI.h"
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
// The following block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the Foundation_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// Foundation_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
//
#if (defined(_WIN32) || defined(_WIN32_WCE)) && defined(NL_DLL)
#if defined(slFramework_EXPORTS)
#define slFramework_API __declspec(dllexport)
#else
#define slFramework_API __declspec(dllimport)	
#endif
#endif


#if !defined(slFramework_API)
#if !defined(NL_NO_GCC_API_ATTRIBUTE) && defined (__GNUC__) && (__GNUC__ >= 4)
#define slFramework_API __attribute__ ((visibility ("default")))
#else
#define slFramework_API
#endif
#endif




//-----------------------------------------------------------------------------

#endif  // __SLFRAMEWORKAPI_H__
