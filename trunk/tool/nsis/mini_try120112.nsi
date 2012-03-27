# -------------------------------------------------------------------
# 压缩，这部分要放在include前面，否则编译无法通过，至于原因，费解
# -------------------------------------------------------------------
# SetCompressor /SOLID LZMA
SetCompressor LZMA
SetCompressorDictSize 32
SetCompress Auto
SetDatablockOptimize On

# -------------------------------------------------------------------
# 头文件
# -------------------------------------------------------------------
# 官方
!include "FileFunc.nsh"
!include "WordFunc.nsh"
!include "MUI.nsh"
# 自定义
!include "definition.nsh"
!include "selectDisk.nsh"
!include "corruptCheck.nsh"
!include "getMacAddress.nsh"
# -------------------------------------------------------------------
# 设置工作目录，可能用到里面的资源，例如图片，配置信息
# -------------------------------------------------------------------
!cd "${Game_NSISResourcesPath}"

# -------------------------------------------------------------------
# 全局变量
# -------------------------------------------------------------------
var EnterDescription
var Version_Alias

# -------------------------------------------------------------------
# 预定文件
# -------------------------------------------------------------------
ReserveFile "Plugins\InstallOptions.dll"
ReserveFile "Plugins\LangDLL.dll"
ReserveFile "Plugins\StartMenu.dll"
ReserveFile "Plugins\UserInfo.dll"
ReserveFile "Plugins\BGImage.dll"
ReserveFile "Contrib\Graphics\Wizard\${Game_WizardImage}.bmp"
ReserveFile "Contrib\Graphics\Header\${Game_HeaderImage}.bmp"
ReserveFile "Contrib\Graphics\Header\${Game_HeaderImage}-R.bmp"
ReserveFile "Contrib\Modern UI\ioSpecial.ini"

# -------------------------------------------------------------------
# 常规设置
# -------------------------------------------------------------------
XPStyle On
CRCCheck off
ShowInstDetails Show
ShowUninstDetails Show
BrandingText "Copyright(C) 2009，${Game_CompanyName}"
Name "${Game_Title}"
OutFile "${Game_OutFile}"
InstallDirRegKey HKCU "Software\${Game_CompanyName}\${Game_Name}" "Path"

# -------------------------------------------------------------------
# 界面设置
# -------------------------------------------------------------------
!define MUI_ICON "Contrib\Graphics\Icons\${Game_IconFile}-Install.ico"
!define MUI_WELCOMEFINISHPAGE_BITMAP "Contrib\Graphics\Wizard\${Game_WizardImage}.bmp"
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "Contrib\Graphics\Header\${Game_HeaderImage}.bmp"
!define MUI_HEADERIMAGE_BITMAP_RTL "Contrib\Graphics\Header\${Game_HeaderImage}-R.bmp"
!define MUI_ABORTWARNING
!define MUI_WELCOMEPAGE_TITLE_3LINES
!define MUI_FINISHPAGE_TITLE_3LINES
!define MUI_FINISHPAGE_NOAUTOCLOSE
!define MUI_LANGDLL_REGISTRY_ROOT "HKCU"
!define MUI_LANGDLL_REGISTRY_KEY "Software\${Game_CompanyName}\${Game_Name}"
!define MUI_LANGDLL_ALWAYSSHOW
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\${Game_CompanyName}\${Game_Name}"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "StartmenuFolder"
!define MUI_UNICON "Contrib\Graphics\Icons\yzun.ico"
!define MUI_UNWELCOMEFINISHPAGE_BITMAP "Contrib\Graphics\Wizard\${Game_WizardImage}.bmp"
!define MUI_UNFINISHPAGE_NOAUTOCLOSE
!define MUI_UNABORTWARNING
!define MUI_CUSTOMFUNCTION_UNGUIINIT un.InitializeGUI
!define MUI_CUSTOMFUNCTION_GUIINIT InitializeGUI

# !insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "${Game_License}"
!insertmacro MUI_PAGE_DIRECTORY
# !insertmacro MUI_PAGE_STARTMENU Application $STARTMENU_FOLDER
!insertmacro MUI_PAGE_INSTFILES
!define MUI_FINISHPAGE_RUN
!define MUI_FINISHPAGE_RUN_CHECKED
# !define MUI_FINISHPAGE_RUN_TEXT "Start a shortcut"
!define MUI_FINISHPAGE_RUN_FUNCTION "LaunchLink"
!insertmacro MUI_PAGE_FINISH
# !define MUI_FINISHPAGE_NOAUTOCLOSE
# !insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

# -------------------------------------------------------------------
# 语言设置
# -------------------------------------------------------------------
!insertmacro MUI_LANGUAGE "SimpChinese"

Function QuickDirDelete
  System::Call "*(i '$HWNDPARENT', i 0x3, t '$1', i 0, i 0x10, i 0, i 0, i 0)i .r0"
  System::Call "shell32::SHFileOperation(i r0)"
  System::Free $0
FunctionEnd

# -------------------------------------------------------------------
# 安装文件区段 subst i: .
# -------------------------------------------------------------------
Section "extract files"
  SetOutPath "$INSTDIR"
  # File /r "f:\plato\bud\trunk\tool\nsis\game.exe"
  # RMDir /r "$INSTDIR\Scp"
  StrCpy $1 "$INSTDIR\creature"
  Call QuickDirDelete

  File /r "e:\Rocket20100701\Bin\Client\Data\creature"
  # NSISdl::download http://download.nullsoft.com/winamp/client/winamp291_lite.exe $R0
SectionEnd

# -------------------------------------------------------------------
# 创建卸载程序区段
# -------------------------------------------------------------------
Section "write uninstaller"
  SetOutPath "$INSTDIR"
  WriteUninstaller "$INSTDIR\Uninstall.exe"
SectionEnd

# -------------------------------------------------------------------
# 开始菜单区段
# -------------------------------------------------------------------
Section "create shortcut in startup menu"
  SetShellVarContext all
  CreateDirectory "$SMPROGRAMS\${STARTMENU_FOLDER}"
  CreateShortCut "$SMPROGRAMS\${STARTMENU_FOLDER}\${Game_Name}.lnk" "$INSTDIR\${Game_Name}.exe"
  CreateShortCut "$SMPROGRAMS\${STARTMENU_FOLDER}\$(^UninstallCaption).lnk" "$INSTDIR\Uninstall.exe"
  SetShellVarContext current
SectionEnd


# -------------------------------------------------------------------
# 桌面区段
# -------------------------------------------------------------------
Section "create shortcut on desktop"
  SetShellVarContext all
  CreateShortCut "$DESKTOP\${Game_Name}.lnk" "$INSTDIR\${Game_Name}.exe"
  SetShellVarContext current
SectionEnd


# -------------------------------------------------------------------
# 注册区段
# -------------------------------------------------------------------
Section "write reg"
   WriteRegStr HKCU "Software\${Game_CompanyName}\${Game_Name}" "Path" "$INSTDIR"
SectionEnd

# -------------------------------------------------------------------
# 初始化函数
# -------------------------------------------------------------------
Function .onInit
Call IterateDisks
StrCpy $0 "Game.exe"
  DetailPrint "Searching for processes called '$0'"
  KillProc::FindProcesses
  StrCmp $1 "-1" wooops
  DetailPrint "-> Found $0 processes"

  StrCmp $0 "0" completed
  Sleep 1500

  StrCpy $0 "Game.exe"
  DetailPrint "Killing all processes called '$0'"
  KillProc::KillProcesses
  StrCmp $1 "-1" wooops
  DetailPrint "-> Killed $0 processes, faild to kill $1 processes"
  Goto completed

  wooops:
  DetailPrint "-> Error: Something went wrong :-("
  Abort

  completed:
  DetailPrint "Everything went okay :-D"



  InitPluginsDir

  ClearErrors
  UserInfo::GetName
  IfErrors RunTheInstaller
  Pop $0
  UserInfo::GetAccountType
  Pop $1

  StrCmp $1 "Admin" RunTheInstaller
  StrCmp $1 "Power" RunTheInstaller
  MessageBox MB_OK|MB_ICONSTOP|MB_TOPMOST "不允许用户 $\"$0$\" 安装这个应用程序.$\n请向您的管理员请求许可 !!!"
  Quit
  RunTheInstaller:
  !insertmacro MUI_LANGDLL_DISPLAY
  ReadRegStr $INSTDIR HKCU "Software\${Game_CompanyName}\${Game_Name}" "Path"
# MessageBox MB_OK "$INSTDIR"
FunctionEnd

Function InitializeGUI
  SetOutPath "$PLUGINSDIR"
  CreateFont $R0 "Verdana" 22 700
  BGImage::AddText /NOUNLOAD "$(^SetupCaption)" $R0 0xFF 0xFB 0xF0 10 10 -10 -10
  BGImage::Redraw /NOUNLOAD
FunctionEnd

Function .onGUIEnd
  BGImage::Destroy
FunctionEnd



# -------------------------------------------------------------------
# 卸载程序区段
# -------------------------------------------------------------------
Section "Uninstall"
  RMDir /r "$INSTDIR"

  SetShellVarContext all
  RMDir /r "$SMPROGRAMS\${STARTMENU_FOLDER}"
  SetShellVarContext current

  SetShellVarContext all
  Delete "$DESKTOP\${Game_Name}.lnk"
  SetShellVarContext current

  DeleteRegKey HKCU "Software\${Game_CompanyName}\${Game_Name}"
SectionEnd


# -------------------------------------------------------------------
# 卸载程序初始化函数
# -------------------------------------------------------------------
Function un.onInit
  ReadINIStr $Version_Alias $INSTDIR\config.ini Login version_alias
  StrCmp $Version_Alias "YaCe" YaCe NotYace
YaCe:
  ReadINIStr $EnterDescription $INSTDIR\bin\YaCe\config.ini App UnDesc
  Goto NextPart
NotYace:
  ReadINIStr $EnterDescription $INSTDIR\bin\NeiCe\config.ini App UnDesc

NextPart:
  StrCpy $0 "Game.exe"
  KillProc::FindProcesses
  StrCmp $1 "-1" wooops

  StrCmp $0 "0" completed
  Sleep 1500

  StrCpy $0 "Game.exe"
  KillProc::KillProcesses
  StrCmp $1 "-1" wooops
  Goto completed
  wooops:
  Abort
  completed:
  DetailPrint "Everything went okay :-D"

  StrCpy $0 "yzonline_update.exe"
  KillProc::FindProcesses
  StrCmp $1 "-1" wooopsA

  StrCmp $0 "0" completedA
  Sleep 1500

  StrCpy $0 "yzonline_update.exe"
  KillProc::KillProcesses
  StrCmp $1 "-1" wooopsA
  Goto completedA
  wooopsA:
  Abort
  completedA:
  DetailPrint "Everything went okay :-D"

  InitPluginsDir

  ClearErrors
  UserInfo::GetName
  IfErrors RunTheUninstaller
  Pop $0
  UserInfo::GetAccountType
  Pop $1

  StrCmp $1 "Admin" RunTheUninstaller
  StrCmp $1 "Power" RunTheUninstaller
  MessageBox MB_OK|MB_ICONSTOP|MB_TOPMOST "不允许用户 $\"$0$\" 卸载这个应用程序.$\n请向您的管理员请求许可 !!!"
  Quit
  RunTheUninstaller:
  !insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd

Function un.InitializeGUI
  SetOutPath "$PLUGINSDIR"
  CreateFont $R0 "Verdana" 22 700
  BGImage::AddText /NOUNLOAD "$(^SetupCaption)" $R0 0xFF 0xFB 0xF0 10 10 -10 -10
  BGImage::Redraw /NOUNLOAD
FunctionEnd

Function un.onUninstSuccess
  BGImage::Destroy
  ExecShell "open" "http://api.yz.szgla.com/GameInstall/GameUnInstall.aspx$EnterDescription"
FunctionEnd
 
Function .onInstSuccess
  # Call CorruptCheck
 Call GetMacAddress
 # ExecShell "open" "http://api.yz.szgla.com/GameInstall/Install.aspx?GameName=yz3&Mac=$6&Remark=wei" 0 SW_SHOWMINIMIZED
FunctionEnd

Function LaunchLink
  ExecShell "open" "$INSTDIR\${Game_Name}.exe" 0 SW_SHOWMINIMIZED
FunctionEnd
# ===================================================================
# end
# ===================================================================
