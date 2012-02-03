# ===================================================================
# 这个 NSIS 脚本 由 SFX Tool 生成
# 版本   
# 作者：     MuldeR (MuldeR2@GMX.de)
# 访问：http://mulder.at.gg 获取更多信息！
#
#           这个应用程序是基于 NSIS v2.11 by Nullsoft
# ===================================================================


# -------------------------------------------------------------------
# 设置工作目录
# -------------------------------------------------------------------
!cd "C:\Program Files\NSIS\Resources"


# -------------------------------------------------------------------
# 定义变量
# -------------------------------------------------------------------
var STARTMENU_FOLDER
var EnterDescription
var Version_Alias

# -------------------------------------------------------------------
# 选择压缩器
# -------------------------------------------------------------------
# SetCompressor /SOLID LZMA
SetCompressor LZMA
SetCompressorDictSize 32
SetCompress Auto
SetDatablockOptimize On


# -------------------------------------------------------------------
# SFX 定义
# -------------------------------------------------------------------
!define SFX_AppID "{5DE6C873-C7B7-4332-8262-2B45ABAD9CD2}"; 您的 SFX 唯一的 ID (它是实的 GUID)
#!define SFX_BaseDir "E:\Rocket-Material\安装包制作绿色版"; SFX Tool 安装文件夹 - 请勿编辑!
!define SFX_Title "远征Online[极速]"; 安装程序的标题
!define SFX_InstallDir "$PROGRAMFILES\冰川网络\远征Online[极速]"; *默认* 的目标安装目录
!define SFX_OutFile "I:\setup\miniyz_setup.exe"; 生成的安装程序文件
# !define SFX_IconFile "Modern-Default"; 安装程序的图标
!define SFX_IconFile "y"; 安装程序的图标
!define SFX_HeaderImage "yz12"; 标题的图像
# !define SFX_HeaderImage "logo"; 标题的图像
# !define SFX_WizardImage "Llama"; 向导的图像
!define SFX_WizardImage "mm"; 向导的图像
!define SFX_Name "远征Online[极速]"
# -------------------------------------------------------------------
# 预定文件
# -------------------------------------------------------------------
ReserveFile "Plugins\InstallOptions.dll"
ReserveFile "Plugins\LangDLL.dll"
ReserveFile "Plugins\StartMenu.dll"
ReserveFile "Plugins\UserInfo.dll"
ReserveFile "Plugins\BGImage.dll"
ReserveFile "Contrib\Graphics\Wizard\${SFX_WizardImage}.bmp"
ReserveFile "Contrib\Graphics\Header\${SFX_HeaderImage}.bmp"
# ReserveFile "Contrib\Graphics\Header\logo.bmp"
ReserveFile "Contrib\Graphics\Header\${SFX_HeaderImage}-R.bmp"
ReserveFile "Contrib\Modern UI\ioSpecial.ini"
# ReserveFile "C:\Documents and Settings\Administrator\桌面\{BA257683-364A-4EB7-8F37-20328F4D4268}.bmp"
# ReserveFile "C:\Documents and Settings\Administrator\桌面\{EC761AE8-84D1-4B7C-9826-FC254F320008}.bmp"


# -------------------------------------------------------------------
# 常规设置
# -------------------------------------------------------------------
XPStyle On
CRCCheck off
ShowInstDetails Show
ShowUninstDetails Show
BrandingText "Copyright(C) 2009，冰川网络"
Name "${SFX_Title}"
OutFile "${SFX_OutFile}"
InstallDir "${SFX_InstallDir}"
InstallDirRegKey HKCU "Software\冰川网络\${SFX_Name}" "Path"

# -------------------------------------------------------------------
# 现代界面设置
# -------------------------------------------------------------------
!include "MUI.nsh"

!define MUI_ICON "Contrib\Graphics\Icons\${SFX_IconFile}-Install.ico"
!define MUI_WELCOMEFINISHPAGE_BITMAP "Contrib\Graphics\Wizard\${SFX_WizardImage}.bmp"
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "Contrib\Graphics\Header\${SFX_HeaderImage}.bmp"
!define MUI_HEADERIMAGE_BITMAP_RTL "Contrib\Graphics\Header\${SFX_HeaderImage}-R.bmp"
!define MUI_ABORTWARNING
!define MUI_WELCOMEPAGE_TITLE_3LINES
!define MUI_FINISHPAGE_TITLE_3LINES
!define MUI_FINISHPAGE_NOAUTOCLOSE
!define MUI_LANGDLL_REGISTRY_ROOT "HKCU"
!define MUI_LANGDLL_REGISTRY_KEY "Software\冰川网络\${SFX_Name}"
# !define MUI_LANGDLL_REGISTRY_VALUENAME "InstallLanguage"
!define MUI_LANGDLL_ALWAYSSHOW
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\冰川网络\${SFX_Name}"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "StartmenuFolder"
# !define MUI_LICENSEPAGE_RADIOBUTTONS
!define MUI_UNICON "Contrib\Graphics\Icons\yzun.ico" #"Contrib\Graphics\Icons\${SFX_IconFile}-Uninstall.ico"
!define MUI_UNWELCOMEFINISHPAGE_BITMAP "Contrib\Graphics\Wizard\${SFX_WizardImage}.bmp"
!define MUI_UNFINISHPAGE_NOAUTOCLOSE
!define MUI_UNABORTWARNING
!define MUI_CUSTOMFUNCTION_UNGUIINIT un.InitializeGUI
!define MUI_CUSTOMFUNCTION_GUIINIT InitializeGUI

# !insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "C:\Program Files\NSIS\license_mini.txt"
!insertmacro MUI_PAGE_DIRECTORY
# !insertmacro MUI_PAGE_STARTMENU Application $STARTMENU_FOLDER
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH
# !insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH


# -------------------------------------------------------------------
# 多语言支持
# -------------------------------------------------------------------
!insertmacro MUI_LANGUAGE "SimpChinese"

# -------------------------------------------------------------------
# 安装文件区段
# -------------------------------------------------------------------
Section
  SetOutPath "$INSTDIR"
    File /r "I:\setup_wei\config.ini"
    File /r "I:\setup_wei\updatescheme.xml"
    # File /r "I:\setup_wei\yzonline_update.exe"
    File /r "I:\setup_wei\远征Online.exe"
    # File /r "I:\setup_wei\dbghelp.dll"
    File /r "I:\setup_wei\serverlist.xml"
    # File /r "I:\setup_wei\download"
    File /r "I:\setup_wei\Skin"
    # File /r "I:\setup_wei\Bin"
    # File /r "I:\setup_wei\data"



SectionEnd


# -------------------------------------------------------------------
# 创建卸载程序区段
# -------------------------------------------------------------------
Section
  SetOutPath "$INSTDIR"

  WriteUninstaller "$INSTDIR\Uninstall.exe"
  # WriteRegStr HKCU "Software\冰川网络\${SFX_Name}"  "DisplayName" "${SFX_Title}"
  # WriteRegStr HKCU "Software\冰川网络\${SFX_Name}" "DisplayIcon" "$\"$INSTDIR\Uninstall.exe$\""
  # WriteRegStr HKCU "Software\冰川网络\${SFX_Name}" "UninstallString" "$\"$INSTDIR\Uninstall.exe$\""
  # WriteRegDWORD HKCU "Software\冰川网络\${SFX_Name}" "NoModify" 1
  # WriteRegDWORD HKCU "Software\冰川网络\${SFX_Name}" "NoRepair" 1
SectionEnd


# -------------------------------------------------------------------
# 开始菜单区段
# -------------------------------------------------------------------
Section
  # !insertmacro MUI_STARTMENU_WRITE_BEGIN Application

  SetShellVarContext all
  CreateDirectory "$SMPROGRAMS\$STARTMENU_FOLDER"
  CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\远征Online [极速].lnk" "$INSTDIR\远征Online.exe"
  CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\$(^UninstallCaption).lnk" "$INSTDIR\Uninstall.exe"
  SetShellVarContext current

  # !insertmacro MUI_STARTMENU_WRITE_END
SectionEnd


# -------------------------------------------------------------------
# 桌面区段
# -------------------------------------------------------------------
Section
  SetShellVarContext all
  CreateShortCut "$DESKTOP\远征Online [极速].lnk" "$INSTDIR\远征Online.exe"
  SetShellVarContext current
SectionEnd


# -------------------------------------------------------------------
# 注册区段
# -------------------------------------------------------------------
Section
  # WriteRegStr HKCU "Software\冰川网络\${SFX_Name}" "" "${SFX_Title}"
  WriteRegStr HKCU "Software\冰川网络\${SFX_Name}" "Path" "$INSTDIR"
SectionEnd


# -------------------------------------------------------------------
# 初始化函数
# -------------------------------------------------------------------
Function .onInit
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
FunctionEnd

Function InitializeGUI
  SetOutPath "$PLUGINSDIR"
  # File /oname=BGImage.bmp "C:\Documents and Settings\Administrator\桌面\{BA257683-364A-4EB7-8F37-20328F4D4268}.bmp"
  # BGImage::SetBG /NOUNLOAD /FILLSCREEN "$PLUGINSDIR\BGImage.bmp"
  # Delete "$PLUGINSDIR\BGImage.bmp"
  # SetOutPath "$PLUGINSDIR"
  # File /oname=BGBrandingImage.bmp "C:\Documents and Settings\Administrator\桌面\{EC761AE8-84D1-4B7C-9826-FC254F320008}.bmp"
  # BGImage::AddImage /NOUNLOAD "$PLUGINSDIR\BGBrandingImage.bmp" 4294967022 10
  # Delete "$PLUGINSDIR\BGBrandingImage.bmp"
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

  !insertmacro MUI_STARTMENU_GETFOLDER Application $R0
  SetShellVarContext all
  RMDir /r "$SMPROGRAMS\$R0"
  SetShellVarContext current

  SetShellVarContext all
  Delete "$DESKTOP\远征Online [极速].lnk"
  SetShellVarContext current

  DeleteRegKey HKCU "Software\冰川网络\${SFX_Name}"

SectionEnd


# -------------------------------------------------------------------
# 卸载程序初始化函数
# -------------------------------------------------------------------
Function un.onInit
  # enter description
  ReadINIStr $Version_Alias $INSTDIR\config.ini Login version_alias
  # MessageBox MB_OK|MB_ICONSTOP|MB_TOPMOST "config.ini == $INSTDIR\config.ini"
  # MessageBox MB_OK|MB_ICONSTOP|MB_TOPMOST "Version_Alias == $Version_Alias"
  StrCmp $Version_Alias "YaCe" YaCe NotYace
YaCe:
  ReadINIStr $EnterDescription $INSTDIR\bin\YaCe\config.ini App UnDesc
  Goto NextPart
NotYace:
  # StrCmp $Version_Alias "NeiCe" NeiCe
# NeiCe:
  ReadINIStr $EnterDescription $INSTDIR\bin\NeiCe\config.ini App UnDesc

  # MessageBox MB_OK|MB_ICONSTOP|MB_TOPMOST "EnterDescription == $EnterDescription"
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
  # File /oname=BGImage.bmp "C:\Documents and Settings\Administrator\桌面\{BA257683-364A-4EB7-8F37-20328F4D4268}.bmp"
  # BGImage::SetBG /NOUNLOAD /FILLSCREEN "$PLUGINSDIR\BGImage.bmp"
  # Delete "$PLUGINSDIR\BGImage.bmp"
  # SetOutPath "$PLUGINSDIR"
  # File /oname=BGBrandingImage.bmp "C:\Documents and Settings\Administrator\桌面\{EC761AE8-84D1-4B7C-9826-FC254F320008}.bmp"
  # BGImage::AddImage /NOUNLOAD "$PLUGINSDIR\BGBrandingImage.bmp" 4294967022 10
  # Delete "$PLUGINSDIR\BGBrandingImage.bmp"
  CreateFont $R0 "Verdana" 22 700
  BGImage::AddText /NOUNLOAD "$(^SetupCaption)" $R0 0xFF 0xFB 0xF0 10 10 -10 -10
  BGImage::Redraw /NOUNLOAD
FunctionEnd

Function un.onGUIEnd
  BGImage::Destroy
  # ExecShell "open" "http://api.yz.szgla.com/GameInstall/UnInstall.aspx$EnterDescription"
  # StrCmp $EnterDescription "" 0 +2
  # StrCpy $EnterDescription "?enter=-1&q="
  ExecShell "open" "http://api.yz.szgla.com/GameInstall/GameUnInstall.aspx$EnterDescription"
  # MessageBox MB_OK|MB_ICONSTOP|MB_TOPMOST "http://api.yz.szgla.com/GameInstall/GameUnInstall.aspx$EnterDescription"
FunctionEnd

# Var size
# Function FileCorrupt1
  # # Push "$INSTDIR\data\mpw.mpk"
  # Exch $0
  # Push $1
  # FileOpen $1 $0 "r"
  # # FileSeek $1 0 END $0
  # FileSeek $1 0 END $size
  # FileClose $1
  # Pop $1
  # Exch $0

 # # IntCmpU $0 758438713 equal less greater
 # # IntCmpU $size 758438713 equal less greater
# equal:
  # Goto done
# less:
  # MessageBox MB_OK "错误信息：mpw.mpk文件安装失败($INSTDIR\data\mpw.mpk 大小:$0 bytes,已损坏)，请关闭杀毒软件和防火墙，并尝试重新安装！"
  # Goto done
# greater:
  # Goto done
# done:

# FunctionEnd

# Function FileCorrupt2
  # # Push "$INSTDIR\data\Creature.mpk"
  # Exch $0
  # Push $1
  # FileOpen $1 $0 "r"
  # FileSeek $1 0 END $size
  # FileClose $1
  # Pop $1
  # Exch $0

 # # IntCmpU $size 250965054 equal less greater
# equal:
  # Goto done
# less:
  # MessageBox MB_OK "错误信息：Creature.mpk文件安装失败，请关闭杀毒软件和防火墙，并尝试重新安装！"
  # Goto done
# greater:
  # Goto done
# done:

# FunctionEnd

# Function FileCorrupt3
  # # Push "$INSTDIR\data\maps.mpk"
  # Exch $0
  # Push $1
  # FileOpen $1 $0 "r"
  # FileSeek $1 0 END $size
  # FileClose $1
  # Pop $1
  # Exch $0

 # # IntCmpU $size 180496434 equal less greater
# equal:
  # Goto done
# less:
  # MessageBox MB_OK "错误信息：maps.mpk文件安装失败，请关闭杀毒软件和防火墙，并尝试重新安装！"
  # Goto done
# greater:
  # Goto done
# done:

# FunctionEnd

Function .onInstSuccess
  # Call FileCorrupt1
  # Call FileCorrupt2
  # Call FileCorrupt3
  ExecShell "open" "http://api.yz.szgla.com/GameInstall/Install.aspx?GameName=yz3&Mac=XXX&Remark=wei"
  Exec '"$INSTDIR\远征Online.exe"'
FunctionEnd

# ===================================================================
# 文件末尾
# ===================================================================
