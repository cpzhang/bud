//**************************************************************************/
// Copyright (c) 1998-2007 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
// DESCRIPTION: Appwizard generated plugin
// AUTHOR: 
//***************************************************************************/

#include "MaxGUP.h"
#include "ParticlePreviewTableActionCallback.h"
#include "ParticlePreviewActionItem.h"
#include "Biped2BoneActionItem.h"
#include "OnlyPreviewSelectedActionItem.h"

MaxGUP::MaxGUP()
{

}

MaxGUP::~MaxGUP()
{

}

// Activate and Stay Resident
DWORD MaxGUP::Start( )
{	
	#pragma message(TODO("Do plugin initialization here"))
	
	IMenuBarContext* pMenuBarContext = ( IMenuBarContext*)GetCOREInterface()->GetMenuManager()->GetContext(kMainMenuBar);
	IMenu* pMenu = pMenuBarContext->GetMenu();
	
	{
		IMenuBarContext* pMenuBarContext = ( IMenuBarContext*)GetCOREInterface()->GetMenuManager()->GetContext(kMainMenuBar);
		IMenu* pMenu = pMenuBarContext->GetMenu();

		//
		std::vector<IMenuItem*> _needDeleted;
		for (int i = 0; i != pMenu->NumItems(); ++i)
		{
			IMenuItem* mi = pMenu->GetItem(i);
			std::string miName = mi->GetTitle();
			if (miName == "Unnamed")
			{
				_needDeleted.push_back(mi);
			}
		}
		for(size_t i = 0; i != _needDeleted.size(); ++i)
		{
			IMenuItem* mi = _needDeleted[i];
			pMenu->RemoveItem(mi);
		}
	}

	//
	IMenuItem* pUnnamedMenuItem = GetIMenuItem();   //menu in menu bar...
	pUnnamedMenuItem->SetTitle("Unnamed");

	//add SubMenu
	IMenu* pUnnamedSubMenu = GetIMenu();
	pUnnamedSubMenu->SetTitle("Unnamed");
	
	for (;;)
	{
		if(GetCOREInterface()->GetMenuManager()->RegisterMenu(pUnnamedSubMenu, 0))
		{
			pUnnamedMenuItem->SetSubMenu(pUnnamedSubMenu);

			// Add the item to the main menu bar
			pMenu->AddItem(pUnnamedMenuItem);

			{
				IMenuItem* pParticlePreviewMenuItem = GetIMenuItem();   //menu in menu bar...
				ActionTable* actionTable =  GetCOREInterface()->GetActionManager()->FindTable(kUnwrapActions);
				// preview all
				if (actionTable->IsEnabled(eActionItem_PreviewAll))
				{
					ActionCallback* cb = new ParticlePreviewTableActionCallback();
					if (GetCOREInterface()->GetActionManager()->ActivateActionTable(cb, kUnwrapActions))
					{
						ParticlePreviewActionItem* ai = (ParticlePreviewActionItem*)actionTable->GetAction(eActionItem_PreviewAll);
						pParticlePreviewMenuItem ->SetActionItem(ai);
						pUnnamedSubMenu->AddItem(pParticlePreviewMenuItem );
					}
				}

				// preview selected
				{
					IMenuItem* pParticlePreviewMenuItem = GetIMenuItem();   //menu in menu bar...
					ActionTable* actionTable =  GetCOREInterface()->GetActionManager()->FindTable(kUnwrapActions);
					if (actionTable->IsEnabled(eActionItem_PreviewSelected))
					{
						ActionCallback* cb = new ParticlePreviewTableActionCallback();
						//if (GetCOREInterface()->GetActionManager()->ActivateActionTable(cb, kUnwrapActions))
						{
							OnlyPreviewSelectedActionItem* ai = (OnlyPreviewSelectedActionItem*)actionTable->GetAction(eActionItem_PreviewSelected);
							pParticlePreviewMenuItem ->SetActionItem(ai);
							pUnnamedSubMenu->AddItem(pParticlePreviewMenuItem );
						}
					}
				}
				// biped to bone
				if(0)
				{
					IMenuItem* pParticlePreviewMenuItem = GetIMenuItem();   //menu in menu bar...
					ActionTable* actionTable =  GetCOREInterface()->GetActionManager()->FindTable(kUnwrapActions);
					if (actionTable->IsEnabled(eActionItem_Biped2Bone))
					{
						ActionCallback* cb = new ParticlePreviewTableActionCallback();
						//if (GetCOREInterface()->GetActionManager()->ActivateActionTable(cb, kUnwrapActions))
						{
							Biped2BoneActionItem* ai = (Biped2BoneActionItem*)actionTable->GetAction(eActionItem_Biped2Bone);
							pParticlePreviewMenuItem ->SetActionItem(ai);
							pUnnamedSubMenu->AddItem(pParticlePreviewMenuItem );
						}
					}
				}
			}


			break;
		}
		
		DebugPrint(_T("Unnamed Menu not registered\n"));
		GetCOREInterface()->GetMenuManager()->UnRegisterMenu(pUnnamedSubMenu);
	}

	GetCOREInterface()->GetMenuManager()->UpdateMenuBar();

	#pragma message(TODO("Return if you want remain loaded or not"))
	return GUPRESULT_NOKEEP;
}

void MaxGUP::Stop( ) 
{
	#pragma message(TODO("Do plugin un-initialization here"))
}

DWORD_PTR MaxGUP::Control( DWORD parameter ) {
	return 0;
}

IOResult MaxGUP::Save(ISave *isave)
{
	return IO_OK;
}

IOResult MaxGUP::Load(ILoad *iload)
{
	return IO_OK;
}

