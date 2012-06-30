#include "WindowHelper.h" 

//
namespace Buddha
{
	int WindowHelper::getWindowHeight( HWND hwnd )
	{
		//
		RECT rectWin;
		GetWindowRect(hwnd, &rectWin);

		//
		return rectWin.bottom - rectWin.top;
	}

	int WindowHelper::getWindowWidth( HWND hwnd )
	{
		//
		RECT rectWin;
		GetWindowRect(hwnd, &rectWin);

		//
		return rectWin.right- rectWin.left;
	}

	int WindowHelper::getClientWidth( HWND hwnd )
	{
		//
		RECT rectClient;
		GetClientRect(hwnd, &rectClient);

		//
		return rectClient.right - rectClient.left;
	}

	int WindowHelper::getClientHeight( HWND hwnd )
	{
		//
		RECT rectClient;
		GetClientRect(hwnd, &rectClient);

		//
		return rectClient.bottom- rectClient.top;
	}

	int WindowHelper::getScreenWidth()
	{
		return GetSystemMetrics(SM_CXSCREEN);
	}

	int WindowHelper::getScreenHeight()
	{
		return GetSystemMetrics(SM_CYSCREEN);
	}

	float WindowHelper::getSceenWidthHeightRatio()
	{
		float w = static_cast<float>(getScreenWidth());
		float h = static_cast<float>(getScreenHeight());

		return w / h;
	}

	void WindowHelper::setAccelerationLevel(int level)
	{
		TCHAR originalLevel[10]={0};

		TCHAR regDevicePath[1024]={0};
		TCHAR regDeviceKeyName[]= TEXT("Acceleration.Level");
		DISPLAY_DEVICE  dv;
		HKEY hKey;
		TCHAR *p;
		int i;

		ZeroMemory(&dv,sizeof(DISPLAY_DEVICE));
		dv.cb=sizeof(DISPLAY_DEVICE);
		EnumDisplayDevices(0,0,&dv,0);

		i=0;
		while(dv.DeviceKey[i])
		{
			dv.DeviceKey[i++]=toupper(dv.DeviceKey[i]);
		}

		p=(TCHAR*)_tcsstr(dv.DeviceKey, TEXT("\\SYSTEM"));
		StringCchCopy(regDevicePath, 1024, p + 1);

		if(ERROR_SUCCESS== RegOpenKeyEx(HKEY_LOCAL_MACHINE,regDevicePath,0,KEY_ALL_ACCESS,&hKey))
		{
			i=9;
			RegQueryValueEx(hKey,regDeviceKeyName,0,0,(LPBYTE)originalLevel,(LPDWORD)&i);
			RegSetValueEx(hKey,regDeviceKeyName,0,REG_DWORD,(BYTE*)&level,sizeof(level));
			ChangeDisplaySettings(0,0x40);  //0x40 查MSDN没有找到什么意思，这里直接在OD中照搬。
			RegCloseKey(hKey);
		}
	}

	int WindowHelper::getAccelerationLevel()
	{
		TCHAR originalLevel[10]={0};

		TCHAR regDevicePath[1024]={0};
		TCHAR regDeviceKeyName[]= TEXT("Acceleration.Level");
		DISPLAY_DEVICE  dv;
		HKEY hKey;
		TCHAR *p;
		int i;

		ZeroMemory(&dv,sizeof(DISPLAY_DEVICE));
		dv.cb=sizeof(DISPLAY_DEVICE);
		EnumDisplayDevices(0,0,&dv,0);

		i=0;
		while(dv.DeviceKey[i])
		{
			dv.DeviceKey[i++]=toupper(dv.DeviceKey[i]);
		}

		p=(TCHAR*)_tcsstr(dv.DeviceKey, TEXT("\\SYSTEM"));
		StringCchCopy(regDevicePath, 1024, p + 1);

		if(ERROR_SUCCESS== RegOpenKeyEx(HKEY_LOCAL_MACHINE,regDevicePath,0,KEY_ALL_ACCESS,&hKey))
		{
			i=9;
			RegQueryValueEx(hKey,regDeviceKeyName,0,0,(LPBYTE)originalLevel,(LPDWORD)&i);
			RegCloseKey(hKey);
		}
		return  (int)originalLevel[0];
	}

}