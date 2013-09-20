#if !defined( EDUNET_OPENSTEER_INCLUDED ) && ( EDUNET_INCLUDE_OPENSTEER == 1 )
#undef __EDUNETCOMMON_H__
#endif


#ifndef __EDUNETCOMMON_H__
#define __EDUNETCOMMON_H__

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

#ifndef EDUNET_NO_OPENSTEER_INCLUDES
#  define EDUNET_NO_OPENSTEER_INCLUDES 0
#endif

#ifdef EDUNET_INCLUDE_OPENSTEER
#  undef EDUNET_INCLUDE_OPENSTEER
#endif

#if EDUNET_NO_OPENSTEER_INCLUDES
#  define EDUNET_INCLUDE_OPENSTEER 0
#else
#  define EDUNET_INCLUDE_OPENSTEER 1
#endif

#include "EduNetMemory/EduNetMemory.h"

// undef outside include guards
#undef EDUNET_INCLUDE_OPENSTEER
#undef EDUNET_NO_OPENSTEER_INCLUDES

#endif