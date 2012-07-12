#include "aboutdlg.h"
#include "IPrimitive.h"
#include "canvas.h"
LRESULT CAnimationDlg::OnLbnSelchangeListAnimations(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	// 如何解析参数
	int nSel = mAnimationListBox.GetCurSel();
	// Get the item text

	std::string strItemText(128, 0);
	mAnimationListBox.GetText(nSel, (char*)strItemText.c_str());
	Canvas::getInstancePtr()->setAnimation(strItemText);

	return 0;
}

void CAnimationDlg::update( IPrimitive* p )
{
	while(mAnimationListBox.GetCount())
	{
		mAnimationListBox.DeleteString(0);
	}

	AniMapIterator it = p->getAniMapIterator();
	while(!it.isAtEnd())
	{
		mAnimationListBox.AddString(it.getCurrentKey().c_str());
		++it;
	}
}

LRESULT CAnimationDlg::OnAnimationPlay( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	mIsAnimated = !mIsAnimated;
	mPlay.EnableWindow(mIsAnimated);
	mStop.EnableWindow(!mIsAnimated);
	mCycle.EnableWindow(!mIsAnimated);
	if (Canvas::getInstancePtr()->_model)
	{
		Canvas::getInstancePtr()->_model->playAnimation();
	}
	
	return 0L;
}

LRESULT CAnimationDlg::OnAnimationStop( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	mIsAnimated = !mIsAnimated;
	mPlay.EnableWindow(mIsAnimated);
	mStop.EnableWindow(!mIsAnimated);
	mCycle.EnableWindow(!mIsAnimated);
	if (Canvas::getInstancePtr()->_model)
	{
		Canvas::getInstancePtr()->_model->stopAnimation();
	}
	
	return 0L;
}

LRESULT CAnimationDlg::OnAnimationCycle( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	static int c = mCycle.GetCheck();
	c = !c;
	mCycle.SetCheck(c);
	return 0L;
}

LRESULT CAnimationDlg::OnNMThemeChangedSliderSpeed( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	int nPercent;
	bHandled = FALSE;

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
		nPercent = m_TrackBar.GetPos();
		bHandled = TRUE;
		break;
	default:
		break;
	} // end switch

	float speed;
	if (nPercent <= 50)
	{
		speed = (float)nPercent / 50.0f;
	}
	else
	{
		speed = -8.0f + (float)nPercent * 9.0f / 50.0f;
	}
	
	if (Canvas::getInstancePtr()->_model)
	{
		Canvas::getInstancePtr()->_model->setSpeed(speed);
	}
	
	return bHandled;
}

void CInformationDlg::update( IPrimitive* p )
{

}

LRESULT CInformationDlg::OnTcnSelchangeTabInformation(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码

	return 0;
}
