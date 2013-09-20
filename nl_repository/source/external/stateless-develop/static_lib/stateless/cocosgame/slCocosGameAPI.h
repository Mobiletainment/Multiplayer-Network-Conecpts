#pragma once
#ifndef __SLCOCOSGAMEAPI_H__
#define __SLCOCOSGAMEAPI_H__

//!---------------------------------------------------------------------------
//! \file "slCocosGameAPI.h"
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


// notes: right now cocosgame is a static lib
//        but later it might become a dynamic lib
//        so for now we skip the stuff below
#define slCocosGame_API

#if 0
#if (defined(_WIN32) || defined(_WIN32_WCE)) && defined(NL_DLL)
#if defined(slCocosGame_EXPORTS)
#define slCocosGame_API __declspec(dllexport)
#else
#define slCocosGame_API __declspec(dllimport)	
#endif
#endif


#if !defined(slCocosGame_API)
#if !defined(NL_NO_GCC_API_ATTRIBUTE) && defined (__GNUC__) && (__GNUC__ >= 4)
#define slCocosGame_API __attribute__ ((visibility ("default")))
#else
#define slCocosGame_API
#endif
#endif
#endif



//-----------------------------------------------------------------------------

#endif  // __SLCOCOSGAMEAPI_H__
