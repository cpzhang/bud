#include "CMainWindow.h"

CMainWindow::CMainWindow()
{

}
LRESULT CMainWindow::onPaint(UINT, WPARAM, LPARAM, BOOL& b) 
{
  b = false;
  return 0;
}
LRESULT CMainWindow::onCreate(UINT, WPARAM, LPARAM, BOOL&) 
{
  //
  new EventManager();
  //
  CreateSimpleToolBar();

  //
  //CreateSimpleStatusBar(TEXT("Ready"));
  m_hWndStatusBar = m_wndStatusBar.Create ( *this );
  // Create the status bar panes.
  int anPanes[] = { ID_DEFAULT_PANE, ID_DEFAULT_PANE_1, 
		    ID_DEFAULT_PANE_2 };

  m_wndStatusBar.SetPanes ( anPanes, 3, true );

  //
  m_wndSplitVertical.Create ( *this, rcDefault );
	
  //
  if (1)
    {
      mPropertyDlg.Create(m_wndSplitVertical, rcDefault);
      m_wndSplitVertical.SetSplitterPane(SPLIT_PANE_LEFT, mPropertyDlg);
    } 
  else
    {
      mTreeDlg.Create(m_wndSplitVertical, rcDefault);
      m_wndSplitVertical.SetSplitterPane(SPLIT_PANE_LEFT, mTreeDlg);
    }
		
  //
  m_wndSplitVerticalProperty.Create(m_wndSplitVertical, rcDefault);
  m_wndSplitVertical.SetSplitterPane(SPLIT_PANE_RIGHT, m_wndSplitVerticalProperty);
  //
  mViewWindow.Create(m_wndSplitVerticalProperty, rcDefault);
  m_wndSplitVerticalProperty.SetSplitterPane(SPLIT_PANE_LEFT, mViewWindow);
  //
  if (1)
    {
      mTreeDlg.Create(m_wndSplitVerticalProperty, rcDefault);
      m_wndSplitVerticalProperty.SetSplitterPane(SPLIT_PANE_RIGHT, mTreeDlg);
    } 
  else
    {
      mPropertyDlg.Create(m_wndSplitVerticalProperty, rcDefault);
      m_wndSplitVerticalProperty.SetSplitterPane(SPLIT_PANE_RIGHT, mPropertyDlg);
    }
		
  //
  m_hWndClient = m_wndSplitVertical;
  UpdateLayout();
  /*
    Note that you need to set m_hWndClient and call CFrameWindowImpl::UpdateLayout() before setting the splitter position. 
    UpdateLayout() resizes the splitter window to its initial size. 
    If you skip that step, the splitter's size isn't under your control and it might be smaller than 200 pixels wide. 
    The end result would be that SetSplitterPos() wouldn't have the effect you wanted.
  */
  m_wndSplitVertical.SetSplitterPos(200);
  m_wndSplitVerticalProperty.SetSplitterPos(700);
  //
  return 1;
}
LRESULT CMainWindow::OnHelpAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  // TODO: 在此添加命令处理程序代码
  CAboutDlg ad;
  ad.DoModal();
  return 0;
}

LRESULT CMainWindow::OnTVSelChanged(int, LPNMHDR pnmh, BOOL&)
{
  // 如何解析参数
  LPNMTREEVIEW pnmtv = reinterpret_cast<LPNMTREEVIEW>(pnmh);
  CTreeViewCtrlEx tree = pnmh->hwndFrom;
  if (tree.IsWindow())
    {
      CTreeItem item = tree.GetSelectedItem();
      std::string path;
      for (; !item.IsNull(); item = item.GetParent())
	{
	  std::string name(256, 0);
	  item.GetText((LPTSTR)name.c_str(), 256);
	  std::string temp = name.c_str();
	  if (!path.empty())
	    {
	      path = temp + "/" + path;
	    }
	  else
	    {
	      path = temp;
	    }
	}
      //
      m_wndStatusBar.SetPaneText(ID_DEFAULT_PANE, TEXT(path.c_str()));
      //
      mViewWindow.changeModelFile(path);
    }

  return 0L;
}



LRESULT CMainWindow::OnEditCreateterrain(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  // TODO: 在此添加命令处理程序代码
  CCreateTerrainDlg dlg;
  dlg.DoModal();
  return 0;
}
void CMainWindow::onIdle()
{
  mViewWindow.onIdle();
}

