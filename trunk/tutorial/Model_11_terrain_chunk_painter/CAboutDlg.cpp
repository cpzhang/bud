#include "CAboutDlg.h"
#include "Misc.h"

LRESULT CPainterTextureButton::OnContextMenu( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled )
{
	bHandled = FALSE;
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog fd(true, "texture", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		_T("(*.dds;*.tga;*.png;*.jpg;)\0*.dds;*.tga;*.png;*.jpg;\0All Files (*.*)\0*.*\0"));
	if (fd.DoModal() == IDOK)
	{
		mTextureName = fd.m_szFileName;
		selected(false, true);
	}
	return TRUE;
}

LRESULT CPainterTextureButton::OnLeftButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	if (selected(true, true))
	{
		//
		EventArgsChunTextureSelected args;
		args.mChunkTextureLayer = mID;
		EventManager::getInstancePtr()->fireEvent(eEvent_ChunkTextureLayerSelected, &args);
	}
	
	return TRUE;
}

bool CPainterTextureButton::selected( bool b, bool forced )
{
	if (mTextureName.empty())
	{
		return false;
	}
	if (!forced && b == mSelected)
	{
		return true;
	}
	mSelected = b;
	return skinButton(mTextureName, *this, b);
}
