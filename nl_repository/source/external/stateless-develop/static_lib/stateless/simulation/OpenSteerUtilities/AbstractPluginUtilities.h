#ifndef __ABSTRACTPLUGINUTILITIES_H__
#define __ABSTRACTPLUGINUTILITIES_H__

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

#include "OpenSteerUtilities/OpenSteerUTTypes.h"

class GLUI_Panel;
class GLUI_Control;
class GLUI;

//-----------------------------------------------------------------------------
namespace OpenSteer
{

#if SL_HAS_GLUI
	//-------------------------------------------------------------------------
	class AbstractPluginGui
	{
	public:
		static void addCommonPluginGui( GLUI_Panel* pluginPanel, osAbstractPlugin* plugin );
		static void changePluginBoolValue( GLUI_Control* control );
		static GLUI_Panel* initRootPluginGui( osAbstractPlugin* plugin );
		static GLUI_Panel* initSubPluginGui( osAbstractPlugin* plugin, void* userdata );
	};
#endif

	template <class Super>
	class PluginAccessorMixin : public Super
	{
		ET_DECLARE_BASE( Super )
	public:

		// constructor
		PluginAccessorMixin():_plugin( NULL ) {};

		// destructor
		virtual ~PluginAccessorMixin()	{};


		virtual void setPlugin( class AbstractPlugin* pPlugin )
		{
			_plugin = pPlugin;
		}

		virtual AbstractPlugin* getPlugin( void ) const
		{
			return _plugin;
		}

	private:
		class AbstractPlugin* _plugin;

	};

	typedef PluginAccessorMixin<Entity> PluginAccessor_t;

	// an pure virtual interface to support plugin selections
	class VirtualPluginSelector
	{
	public:
		virtual ~VirtualPluginSelector()	{};

		virtual const char* getCurrentPluginName( void ) const ET_ABSTRACT;
		virtual void selectPluginByName( const char* pszPluginName ) ET_ABSTRACT;
	};

}

#endif //  __ABSTRACTPLUGINUTILITIES_H__