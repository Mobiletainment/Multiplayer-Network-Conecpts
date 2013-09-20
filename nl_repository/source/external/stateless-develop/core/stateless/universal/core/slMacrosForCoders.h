#pragma once
#ifndef __SLMACROSFORCODERS_H__
#define __SLMACROSFORCODERS_H__
//!---------------------------------------------------------------------------
//! \file slMacrosForCoders.h
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2008-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------


// Andreas macros
#define SL_CODERVERSION_AB 0
#define SL_CODERVERSION_AB_TEST 0

// Cyrus macros
#define SL_CODERVERSION_CP 0
#define SL_CODERVERSION_CP_TEST 0

// Carsten macros
#define SL_CODERVERSION_CK 0
#define SL_CODERVERSION_CK_TEST 0

// Sebastian macros
#define SL_CODERVERSION_SA 0
#define SL_CODERVERSION_SA_TEST 0

// Volker macros
#define SL_CODERVERSION_VA 0
#define SL_CODERVERSION_VA_TEST 0

//------------------------------------------------------------------------------

// section for Andreas
#if SL_CODERVERSION_AB
// i want the gl debug view
//#  define SL_HAVE_GL_DEBUGVIEW 1
#endif


// section for Cyrus
#if SL_CODERVERSION_CP
// i want the gl debug view
#  define SL_HAVE_GL_DEBUGVIEW 1
#endif


// section for Carsten
#if SL_CODERVERSION_CK
// i want the gl debug view
//#  define SL_HAVE_GL_DEBUGVIEW 1
#endif


// section for Sebastian
#if SL_CODERVERSION_SA
// i want the gl debug view
//#  define SL_HAVE_GL_DEBUGVIEW 1
#endif


// section for Volker
#if SL_CODERVERSION_VA
// i want the gl debug view
//#  define SL_HAVE_GL_DEBUGVIEW 1
#endif

//------------------------------------------------------------------------------

#if (SL_CODERVERSION_AB | SL_CODERVERSION_CP | SL_CODERVERSION_CK | SL_CODERVERSION_SA | SL_CODERVERSION_VA)
#  define SL_DEVELOPERVERSION 1
#else
#  define SL_DEVELOPERVERSION 0
#endif

//------------------------------------------------------------------------------

#ifndef SL_HAVE_GL_DEBUGVIEW
#  define SL_HAVE_GL_DEBUGVIEW 0
#endif

#endif // __SLMACROSFORCODERS_H__
