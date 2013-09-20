#include "stdafx.h"
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

#include "OpenSteerUtilities/AbstractPluginUtilities.h"
#if SL_HAS_GLUI
#  include "EduNetCommon/EduNetDraw.h"
#endif
#include "OpenSteer/GlobalData.h"

#define ET_CTRL_ID_VISIBLE 1
#define ET_CTRL_ID_ENABLED 2

//-----------------------------------------------------------------------------
namespace OpenSteer
{

#if SL_HAS_GLUI
	//-------------------------------------------------------------------------
	void AbstractPluginGui::changePluginBoolValue( GLUI_Control* pkControl )
	{
		osAbstractEntity* pkPluginEntity = (osAbstractEntity*)pkControl->ptr_val;
		if(NULL == pkPluginEntity)
		{
			return;
		}

		AbstractUpdated* pkUpdated = dynamic_cast<AbstractUpdated*>( pkPluginEntity );

		GLUI_Checkbox* pkCheckBox = (GLUI_Checkbox*)pkControl;
		const bool bValue = pkCheckBox->get_int_val() == 1 ? true : false;
		switch( pkControl->get_id() )
		{
		case ET_CTRL_ID_VISIBLE: 
			{
				pkPluginEntity->setVisible( bValue );
			}
			break;
		case ET_CTRL_ID_ENABLED: 
			{
				if( NULL != pkUpdated )
				{
					pkUpdated->setEnabled( bValue );
				}
			}
			break;
		}
	}

	//-------------------------------------------------------------------------
	void AbstractPluginGui::addCommonPluginGui( GLUI_Panel* pluginPanel, osAbstractPlugin* pkPlugin )
	{
		// enable/disable
		// visibility
		GLUI* glui = ::getRootGLUI();
		GLUI_Rollout* pluginSettingsRollout = glui->add_rollout_to_panel( pluginPanel, "Settings", false );
		GLUI_Panel* subPanel = pluginSettingsRollout;

		osAbstractEntity* pkPluginEntity = OpenSteer::CastToAbstractEntity( pkPlugin );
		int visible = pkPluginEntity->isVisible() ? 1 : 0;
		GLUI_Checkbox* pkControl = 
			glui->add_checkbox_to_panel( subPanel, "Visible", NULL, ET_CTRL_ID_VISIBLE, AbstractPluginGui::changePluginBoolValue );
		pkControl->set_ptr_val( pkPluginEntity );
		pkControl->set_int_val( visible );

		AbstractUpdated* pkUpdated = dynamic_cast<AbstractUpdated*>( pkPluginEntity );
		if( NULL != pkUpdated )
		{
			int enabled = pkUpdated->isEnabled() ? 1 : 0;
			pkControl = 
				glui->add_checkbox_to_panel( subPanel, "Enable", NULL, ET_CTRL_ID_ENABLED, AbstractPluginGui::changePluginBoolValue );
			pkControl->set_ptr_val( pkPluginEntity );
			pkControl->set_int_val( enabled );
		}

		pkControl = 
			glui->add_checkbox_to_panel( subPanel, "Network Statistics", &OpenSteer::GlobalData::getInstance()->m_bDebugNetStats );

	}

	//-------------------------------------------------------------------------
	GLUI_Panel* AbstractPluginGui::initRootPluginGui( osAbstractPlugin* pkPlugin )
	{
		GLUI* glui = ::getRootGLUI();

		GLUI_Rollout* pluginRollout = glui->add_rollout( pkPlugin ? pkPlugin->pluginName() : "Plugin", false );	
		GLUI_Panel* pluginPanel = pluginRollout;
		AbstractPluginGui::addCommonPluginGui( pluginPanel, pkPlugin );
		pkPlugin->initGui( pluginPanel );
		return pluginPanel;
	}

	//-------------------------------------------------------------------------
	GLUI_Panel* AbstractPluginGui::initSubPluginGui( osAbstractPlugin* pkPlugin, void* pkUserdata )
	{
		GLUI* glui = ::getRootGLUI();
		GLUI_Panel* pluginPanel = static_cast<GLUI_Panel*>(pkUserdata);

		GLUI_Rollout* pluginRollout = glui->add_rollout_to_panel( pluginPanel, pkPlugin ? pkPlugin->pluginName() : "Plugin", false );	
		GLUI_Panel* subPluginPanel = pluginRollout;
		AbstractPluginGui::addCommonPluginGui( subPluginPanel, pkPlugin );
		pkPlugin->initGui( subPluginPanel );

		return subPluginPanel;
	}
#endif

}

