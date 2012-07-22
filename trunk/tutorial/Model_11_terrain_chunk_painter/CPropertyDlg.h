#pragma once
#include "atlHeaders.h"
#include "EventArgs.h"
#include "CAboutDlg.h"
class CPropertyDlg : public CDialogImpl<CPropertyDlg>
{
public:
	enum { IDD = IDD_FORMVIEW_PROPERTY };

	BEGIN_MSG_MAP(CPropertyDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_CONTEXTMENU, OnContextMenu)
		MESSAGE_HANDLER(WM_HSCROLL, OnNMThemeChangedSlider)
		NOTIFY_HANDLER(IDC_TREE1, NM_RCLICK, OnNMRclickTree1)
		COMMAND_ID_HANDLER(ID_MATERIAL_SKINNEDANIMATION, OnMaterialSkinnedanimation)
		COMMAND_ID_HANDLER(ID_MATERIAL_MESH, OnMaterialMesh)
		COMMAND_ID_HANDLER(ID_MATERIAL_MATERIAL, OnMaterialMaterial)
		COMMAND_HANDLER(IDC_BUTTON_ColorPicker, BN_CLICKED, OnBnClickedButtonColorpicker)
		COMMAND_HANDLER(IDC_CHECK_FillMode, BN_CLICKED, OnBnClickedCheckFillmode)
	END_MSG_MAP()

	LRESULT OnNMThemeChangedSlider( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnContextMenu(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnNMRclickTree1(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
	LRESULT OnMaterialSkinnedanimation(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	LRESULT OnMaterialMesh(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	LRESULT OnMaterialMaterial(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
private:
	void selectPainterTexture(int nID);
	bool updateName(const EventArgs& e);
	bool updateChunTextureSelected(const EventArgs& e);
private:
	CTreeViewCtrlEx m_tvCtrl;
	CMenu mMenu;
	CTrackBarCtrl mTrackBarSculptorRadius;
	CTrackBarCtrl mTrackBarSculptorStrength;
	CTrackBarCtrl mTrackBarPainterAlpha;
	CPainterTextureButton mPainterTextureButton[eChunkTextureLayer_Size];
	eChunkTextureLayer mChunkTextureLayer;
private:
	LRESULT OnBnClickedButtonColorpicker(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
public:
	LRESULT OnBnClickedCheckFillmode(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};