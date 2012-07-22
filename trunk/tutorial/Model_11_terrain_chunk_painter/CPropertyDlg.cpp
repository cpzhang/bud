#include "CPropertyDlg.h"
#include "render/Euclid.h"
#include "SubEntity.h"
#include "Entity.h"
#include "Misc.h"

#include <atlbase.h>
#include <atlapp.h>
#include <atlctrls.h>
#include <atlframe.h>
#include <atlctrlx.h>
#include <atlddx.h>
LRESULT CPropertyDlg::OnNMThemeChangedSlider( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	int nPercent;
	bHandled = FALSE;

	if ((HWND)lParam == mTrackBarSculptorRadius)
	{
		switch ((int)LOWORD(wParam)) 
		{
		case SB_THUMBTRACK:
		case SB_THUMBPOSITION:
			nPercent = (short)HIWORD(wParam);
			bHandled = TRUE;
			break;
		case SB_ENDSCROLL:
		case SB_LINELEFT:
		case SB_LINERIGHT:
		case SB_PAGELEFT:
		case SB_PAGERIGHT:
			nPercent = mTrackBarSculptorRadius.GetPos();
			bHandled = TRUE;
			break;
		default:
			break;
		} // end switch
		EventArgsSculptorRadiusChanged arg;
		arg.mRadius = nPercent;
		EventManager::getInstancePtr()->fireEvent(eEvent_SculptorRadiusChanged, &arg);		
	}
	//
	if ((HWND)lParam == mTrackBarSculptorStrength)
	{
		switch ((int)LOWORD(wParam)) 
		{
		case SB_THUMBTRACK:
		case SB_THUMBPOSITION:
			nPercent = (short)HIWORD(wParam);
			bHandled = TRUE;
			break;
		case SB_ENDSCROLL:
		case SB_LINELEFT:
		case SB_LINERIGHT:
		case SB_PAGELEFT:
		case SB_PAGERIGHT:
			nPercent = mTrackBarSculptorStrength.GetPos();
			bHandled = TRUE;
			break;
		default:
			break;
		} // end switch
		EventArgsSculptorStrengthChanged arg;
		arg.mStrength = float(nPercent) / 100.0f;
		EventManager::getInstancePtr()->fireEvent(eEvent_SculptorStrengthChanged, &arg);		
	}
	//
	//
	if ((HWND)lParam == mTrackBarPainterAlpha)
	{
		switch ((int)LOWORD(wParam)) 
		{
		case SB_THUMBTRACK:
		case SB_THUMBPOSITION:
			nPercent = (short)HIWORD(wParam);
			bHandled = TRUE;
			break;
		case SB_ENDSCROLL:
		case SB_LINELEFT:
		case SB_LINERIGHT:
		case SB_PAGELEFT:
		case SB_PAGERIGHT:
			nPercent = mTrackBarPainterAlpha.GetPos();
			bHandled = TRUE;
			break;
		default:
			break;
		} // end switch
		CButton b = GetDlgItem(IDC_BUTTON_PainterAlpha);
		{
			DeleteObject(b.GetBitmap());
			std::vector<COLORREF> cs;
			// 0x00bbggrr
			Real c = float(nPercent)/100.0f;
			Zen::Color col;
			col.a = 1.0f;
			size_t width = 120;
			size_t height = 120;
			cs.resize(width * height, col.getARGB());
			for (size_t x = 0; x != width; ++x)
				for (size_t y = 0; y != height; ++y)
				{
					float distanceSquare = (x - width * 0.5f) * (x - width * 0.5f) + (y - height * 0.5f) * (y - height * 0.5f);
					float norm = distanceSquare / (float)(width * width);
					float v = Zen::Basic::Cos(norm * Zen::HalfPI);
					if (c == 0.0f)
					{
						v = 1.0f;
					}
					col.r = col.g = col.b = max(0.0f, v - c);
					cs[x + y * width] = col.getARGB();
				}
				HBITMAP h = CreateBitmap(width, height, 1, 32, &cs[0]);
				b.SetBitmap(h);
		}
	}

	return bHandled;
}

LRESULT CPropertyDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	//
	mMenu.LoadMenu(IDR_MENU2);

	//
	mTrackBarSculptorRadius = GetDlgItem(IDC_SLIDER1);
	mTrackBarSculptorStrength = GetDlgItem(IDC_SLIDER2);
	mTrackBarPainterAlpha = GetDlgItem(IDC_SLIDER_PainterAlpha);

	//
	EventManager::getInstancePtr()->subscribeEvent(eEvent_CTreeDlgChangeFile, SubscriberSlot(&CPropertyDlg::updateName, this));
	EventManager::getInstancePtr()->subscribeEvent(eEvent_ChunkTextureLayerSelected, SubscriberSlot(&CPropertyDlg::updateChunTextureSelected, this));
	//
	m_tvCtrl = GetDlgItem(IDC_TREE1);
	//
	CenterWindow();
	ShowWindow(SW_NORMAL);
	//
	for (size_t i = 0; i != eChunkTextureLayer_Size; ++i)
	{
		mPainterTextureButton[i].SubclassWindow(GetDlgItem(IDC_BUTTON_Texture0 + i));
		mPainterTextureButton[i].mID = (eChunkTextureLayer)i;
	}
	
	//
	return TRUE;
}
LRESULT CPropertyDlg::OnContextMenu(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	// Cursor's horizontal and vertical position
	POINT pt;
	pt.x = GET_X_LPARAM(lParam);
	pt.y = GET_Y_LPARAM(lParam); 
	//
	//mMenu.GetSubMenu(0).TrackPopupMenu(TPM_LEFTALIGN|TPM_TOPALIGN|TPM_LEFTBUTTON, pt.x, pt.y,m_hWnd);

	return TRUE;
}
LRESULT CPropertyDlg::OnNMRclickTree1(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	// 如何解析参数
	LPNMTREEVIEW pnmtv = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	CTreeViewCtrlEx tree = pNMHDR->hwndFrom;
	std::string name(256, 0);
	if (tree.IsWindow())
	{
		CTreeItem item = tree.GetSelectedItem();
		if (!item.IsNull())
		{
			item.GetText((LPTSTR)name.c_str(), 256);
			// Cursor's horizontal and vertical position
			POINT pt;
			pt.x = GET_X_LPARAM(GetMessagePos());
			pt.y = GET_Y_LPARAM(GetMessagePos()); 
			//
			mMenu.GetSubMenu(0).TrackPopupMenu(TPM_LEFTALIGN|TPM_TOPALIGN|TPM_LEFTBUTTON, pt.x, pt.y,m_hWnd);
		}
	}

	return 0;
}

LRESULT CPropertyDlg::OnMaterialSkinnedanimation(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加命令处理程序代码

	return 0;
}

LRESULT CPropertyDlg::OnMaterialMesh(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加命令处理程序代码

	return 0;
}

LRESULT CPropertyDlg::OnMaterialMaterial(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加命令处理程序代码

	return 0;
}
bool CPropertyDlg::updateName(const EventArgs& e)
{
	EventArgsCTreeDlgChangeFile args = (EventArgsCTreeDlgChangeFile&)e;
	std::string name("unknown");
	switch (args.mType)
	{
	case eVisibleObjectType_SubEntity:
		{
			SubEntity* e = (SubEntity*)args.mPrimitive;
			name = e->mName;
			//
			{
				m_tvCtrl.DeleteAllItems();
				CTreeItem t;
				CTreeItem tail;						
				//如何插入根节点
				{
					t = m_tvCtrl.InsertItem ( NULL, TVI_ROOT, TVI_LAST);
					t.SetText(name.c_str());
				}
			}
		};
		break;
	case eVisibleObjectType_Entity:
		{
			Entity* e = (Entity*)args.mPrimitive;
			name = e->mName;
			//
			{
				m_tvCtrl.DeleteAllItems();
				CTreeItem t;
				CTreeItem tail;						
				//如何插入根节点
				{
					t = m_tvCtrl.InsertItem ( NULL, TVI_ROOT, TVI_LAST);
					t.SetText(name.c_str());
				}
				//如何插入子节点
				{
					for (size_t i = 0; i != e->mSubEntities.size(); ++i)
					{
						name = e->mSubEntities[i]->mName;
						t.AddTail(name.c_str(), 0);
					}
				}
			}

		};
		break;
	}
	return true;
}
void CPropertyDlg::selectPainterTexture(int nID)
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog fd(true, "texture", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		_T("(*.dds;*.tga;*.png;*.jpg;)\0*.dds;*.tga;*.png;*.jpg;\0All Files (*.*)\0*.*\0"));
	if (fd.DoModal() == IDOK)
	{
		skinButton(fd.m_szFileName, (CButton)GetDlgItem(nID));
	}
}
LRESULT CPropertyDlg::OnBnClickedButtonColorpicker(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	CColorDialog d;
	d.DoModal();
	COLORREF c = d.GetColor();
	CButton b = GetDlgItem(IDC_BUTTON_ColorPicker);
	{
		std::vector<COLORREF> cs;
		size_t width = 72;
		size_t height = 40;
		cs.resize(width * height, c);
		HBITMAP h = CreateBitmap(width, height, 1, 32, &cs[0]);
		b.SetBitmap(h);
	}
	return 0;
}

bool CPropertyDlg::updateChunTextureSelected( const EventArgs& e )
{
	EventArgsChunTextureSelected* args = (EventArgsChunTextureSelected*)&e;
	mChunkTextureLayer = args->mChunkTextureLayer;
	for (size_t i = 0; i != eChunkTextureLayer_Size; ++i)
	{
		if (mChunkTextureLayer != i)
		{
			mPainterTextureButton[i].selected(false);
		}
	}
	return true;
}

LRESULT CPropertyDlg::OnBnClickedCheckFillmode(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	if (gTerrain.getMaterial() == NULL)
	{
		return 0;
	}
	if (gTerrain.getMaterial()->mFillMode == Zen::eFillMode_WireFrame)
	{
		gTerrain.getMaterial()->mFillMode = Zen::eFillMode_Solid;
	} 
	else
	{
		gTerrain.getMaterial()->mFillMode = Zen::eFillMode_WireFrame;
	}
	return 0;
}
