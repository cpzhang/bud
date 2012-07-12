#include <atlbase.h>
#include <atlapp.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <string>
#include "resource.h"
#include "inc/fmod.hpp"
CAppModule _Module;
class CMainWindow : public CDialogImpl<CMainWindow>
{
public:
	enum { IDD = IDD_FORMVIEW };

	BEGIN_MSG_MAP(CMainWindow)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_HANDLER(ID_PLAY, BN_CLICKED, OnBnClickedPlay)
		COMMAND_HANDLER(ID_STOP, BN_CLICKED, OnBnClickedStop)
		COMMAND_HANDLER(IDC_BUTTON_FILE, BN_CLICKED, OnBnClickedButtonFile)
	END_MSG_MAP()

	CMainWindow()
	{
		mSystem = NULL;
		mSound = NULL;
		mChannel = NULL;
	}
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CenterWindow();

		//
		{
			mLibVersions = GetDlgItem(IDC_LIST1);
			mLibVersions.InsertString(0, "0.0");
			mLibVersions.InsertString(1, "0.1");
		}
		//
		{
			mFileButton = GetDlgItem(IDC_BUTTON_FILE);
		}
		//
		{
			FMOD_RESULT result;
			
			result = FMOD::System_Create(&mSystem);
			bool bHardware = false;
			FMOD_CAPS caps;
			result = mSystem->getDriverCaps(0, &caps, 0, 0, 0);
			if (caps & FMOD_CAPS_HARDWARE)
			{
			}
			result = mSystem->init(100, FMOD_INIT_NORMAL, 0);
		}
		//
		return TRUE;
	}

	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		mSound->release();
		mSystem->release();
		PostQuitMessage(0);
		EndDialog(0);
		return 0;
	}
public:
	LRESULT OnBnClickedPlay(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: 在此添加控件通知处理程序代码
		TCHAR buffer[1024] = {0};
		mFileButton.GetWindowText(buffer, 1024);
		FMOD_RESULT result;
		if (mSound != NULL)
		{
			mSound->release();
			mSound = NULL;
		}
		result = mSystem->createSound(buffer, FMOD_HARDWARE, 0, &mSound);
		if (result != FMOD_OK)
		{
			return -1;
		}

		//Number of times to loop before stopping. 0 = oneshot. 1 = loop once then stop. -1 = loop forever. Default = -1 
		mSound->setLoopCount(-1);
		result = mSystem->playSound(FMOD_CHANNEL_FREE, mSound, FALSE, &mChannel);

		mChannel->setMode(FMOD_LOOP_NORMAL);

		if (result != FMOD_OK)
		{
			return -1;
		}

		return 0;
	}

	LRESULT OnBnClickedStop(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: 在此添加控件通知处理程序代码
		if (mChannel)
		{
			mChannel->stop();
		}
		return 0;
	}
	LRESULT CMainWindow::OnBnClickedButtonFile(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: 在此添加控件通知处理程序代码
		// TODO: add code to initialize document
		CFileDialog fd(true, "声音文件", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("声音文件 (*.mp3;*.wav;)\0*.mp3;*.wav;\0All Files (*.*)\0*.*\0"));
		if (fd.DoModal() == IDOK)
		{
			std::string buf(512, 0);
			buf = fd.m_szFileName;
			if (!buf.empty())
			{
				mFileButton.SetWindowText(fd.m_szFileName);
			}
			else
			{
				mFileButton.SetWindowText(TEXT("非有效文件"));
			}
		}
		return 0;

	}
private:
	CListBox mLibVersions;
	CButton  mFileButton;
	FMOD::System* mSystem;
	FMOD::Sound* mSound;
	FMOD::Channel* mChannel;
};

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE , LPTSTR lpstrCmdLine, int nCmdShow)
{
	CMainWindow mw;
	mw.Create(NULL);
	mw.CenterWindow();
	mw.ShowWindow(nCmdShow);
	mw.UpdateWindow();
	MSG m;
	::ZeroMemory(&m, sizeof(m));
	for (;;)
	{
		::ZeroMemory(&m, sizeof(m));
		if (::PeekMessage(&m, NULL, 0, 0, PM_NOREMOVE))
		{
			::GetMessage(&m, NULL, 0, 0);
			if (m.message == WM_QUIT)
			{
				break;
			}
			::TranslateMessage(&m);
			::DispatchMessage(&m);
		}
		else
		{

		}
	}
	return 0;
}