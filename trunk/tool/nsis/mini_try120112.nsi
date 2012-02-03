# ===================================================================
# ��� NSIS �ű� �� SFX Tool ����
# �汾   
# ���ߣ�     MuldeR (MuldeR2@GMX.de)
# ���ʣ�http://mulder.at.gg ��ȡ������Ϣ��
#
#           ���Ӧ�ó����ǻ��� NSIS v2.11 by Nullsoft
# ===================================================================


# -------------------------------------------------------------------
# ���ù���Ŀ¼
# -------------------------------------------------------------------
!cd "C:\Program Files\NSIS\Resources"


# -------------------------------------------------------------------
# �������
# -------------------------------------------------------------------
var STARTMENU_FOLDER
var EnterDescription
var Version_Alias

# -------------------------------------------------------------------
# ѡ��ѹ����
# -------------------------------------------------------------------
# SetCompressor /SOLID LZMA
SetCompressor LZMA
SetCompressorDictSize 32
SetCompress Auto
SetDatablockOptimize On


# -------------------------------------------------------------------
# SFX ����
# -------------------------------------------------------------------
!define SFX_AppID "{5DE6C873-C7B7-4332-8262-2B45ABAD9CD2}"; ���� SFX Ψһ�� ID (����ʵ�� GUID)
#!define SFX_BaseDir "E:\Rocket-Material\��װ��������ɫ��"; SFX Tool ��װ�ļ��� - ����༭!
!define SFX_Title "Զ��Online[����]"; ��װ����ı���
!define SFX_InstallDir "$PROGRAMFILES\��������\Զ��Online[����]"; *Ĭ��* ��Ŀ�갲װĿ¼
!define SFX_OutFile "I:\setup\miniyz_setup.exe"; ���ɵİ�װ�����ļ�
# !define SFX_IconFile "Modern-Default"; ��װ�����ͼ��
!define SFX_IconFile "y"; ��װ�����ͼ��
!define SFX_HeaderImage "yz12"; �����ͼ��
# !define SFX_HeaderImage "logo"; �����ͼ��
# !define SFX_WizardImage "Llama"; �򵼵�ͼ��
!define SFX_WizardImage "mm"; �򵼵�ͼ��
!define SFX_Name "Զ��Online[����]"
# -------------------------------------------------------------------
# Ԥ���ļ�
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
# ReserveFile "C:\Documents and Settings\Administrator\����\{BA257683-364A-4EB7-8F37-20328F4D4268}.bmp"
# ReserveFile "C:\Documents and Settings\Administrator\����\{EC761AE8-84D1-4B7C-9826-FC254F320008}.bmp"


# -------------------------------------------------------------------
# ��������
# -------------------------------------------------------------------
XPStyle On
CRCCheck off
ShowInstDetails Show
ShowUninstDetails Show
BrandingText "Copyright(C) 2009����������"
Name "${SFX_Title}"
OutFile "${SFX_OutFile}"
InstallDir "${SFX_InstallDir}"
InstallDirRegKey HKCU "Software\��������\${SFX_Name}" "Path"

# -------------------------------------------------------------------
# �ִ���������
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
!define MUI_LANGDLL_REGISTRY_KEY "Software\��������\${SFX_Name}"
# !define MUI_LANGDLL_REGISTRY_VALUENAME "InstallLanguage"
!define MUI_LANGDLL_ALWAYSSHOW
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\��������\${SFX_Name}"
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
# ������֧��
# -------------------------------------------------------------------
!insertmacro MUI_LANGUAGE "SimpChinese"

# -------------------------------------------------------------------
# ��װ�ļ�����
# -------------------------------------------------------------------
Section
  SetOutPath "$INSTDIR"
    File /r "I:\setup_wei\config.ini"
    File /r "I:\setup_wei\updatescheme.xml"
    # File /r "I:\setup_wei\yzonline_update.exe"
    File /r "I:\setup_wei\Զ��Online.exe"
    # File /r "I:\setup_wei\dbghelp.dll"
    File /r "I:\setup_wei\serverlist.xml"
    # File /r "I:\setup_wei\download"
    File /r "I:\setup_wei\Skin"
    # File /r "I:\setup_wei\Bin"
    # File /r "I:\setup_wei\data"



SectionEnd


# -------------------------------------------------------------------
# ����ж�س�������
# -------------------------------------------------------------------
Section
  SetOutPath "$INSTDIR"

  WriteUninstaller "$INSTDIR\Uninstall.exe"
  # WriteRegStr HKCU "Software\��������\${SFX_Name}"  "DisplayName" "${SFX_Title}"
  # WriteRegStr HKCU "Software\��������\${SFX_Name}" "DisplayIcon" "$\"$INSTDIR\Uninstall.exe$\""
  # WriteRegStr HKCU "Software\��������\${SFX_Name}" "UninstallString" "$\"$INSTDIR\Uninstall.exe$\""
  # WriteRegDWORD HKCU "Software\��������\${SFX_Name}" "NoModify" 1
  # WriteRegDWORD HKCU "Software\��������\${SFX_Name}" "NoRepair" 1
SectionEnd


# -------------------------------------------------------------------
# ��ʼ�˵�����
# -------------------------------------------------------------------
Section
  # !insertmacro MUI_STARTMENU_WRITE_BEGIN Application

  SetShellVarContext all
  CreateDirectory "$SMPROGRAMS\$STARTMENU_FOLDER"
  CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Զ��Online [����].lnk" "$INSTDIR\Զ��Online.exe"
  CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\$(^UninstallCaption).lnk" "$INSTDIR\Uninstall.exe"
  SetShellVarContext current

  # !insertmacro MUI_STARTMENU_WRITE_END
SectionEnd


# -------------------------------------------------------------------
# ��������
# -------------------------------------------------------------------
Section
  SetShellVarContext all
  CreateShortCut "$DESKTOP\Զ��Online [����].lnk" "$INSTDIR\Զ��Online.exe"
  SetShellVarContext current
SectionEnd


# -------------------------------------------------------------------
# ע������
# -------------------------------------------------------------------
Section
  # WriteRegStr HKCU "Software\��������\${SFX_Name}" "" "${SFX_Title}"
  WriteRegStr HKCU "Software\��������\${SFX_Name}" "Path" "$INSTDIR"
SectionEnd


# -------------------------------------------------------------------
# ��ʼ������
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
  MessageBox MB_OK|MB_ICONSTOP|MB_TOPMOST "�������û� $\"$0$\" ��װ���Ӧ�ó���.$\n�������Ĺ���Ա������� !!!"
  Quit
  RunTheInstaller:
  !insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd

Function InitializeGUI
  SetOutPath "$PLUGINSDIR"
  # File /oname=BGImage.bmp "C:\Documents and Settings\Administrator\����\{BA257683-364A-4EB7-8F37-20328F4D4268}.bmp"
  # BGImage::SetBG /NOUNLOAD /FILLSCREEN "$PLUGINSDIR\BGImage.bmp"
  # Delete "$PLUGINSDIR\BGImage.bmp"
  # SetOutPath "$PLUGINSDIR"
  # File /oname=BGBrandingImage.bmp "C:\Documents and Settings\Administrator\����\{EC761AE8-84D1-4B7C-9826-FC254F320008}.bmp"
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
# ж�س�������
# -------------------------------------------------------------------
Section "Uninstall"
  RMDir /r "$INSTDIR"

  !insertmacro MUI_STARTMENU_GETFOLDER Application $R0
  SetShellVarContext all
  RMDir /r "$SMPROGRAMS\$R0"
  SetShellVarContext current

  SetShellVarContext all
  Delete "$DESKTOP\Զ��Online [����].lnk"
  SetShellVarContext current

  DeleteRegKey HKCU "Software\��������\${SFX_Name}"

SectionEnd


# -------------------------------------------------------------------
# ж�س����ʼ������
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
  MessageBox MB_OK|MB_ICONSTOP|MB_TOPMOST "�������û� $\"$0$\" ж�����Ӧ�ó���.$\n�������Ĺ���Ա������� !!!"
  Quit
  RunTheUninstaller:
  !insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd

Function un.InitializeGUI
  SetOutPath "$PLUGINSDIR"
  # File /oname=BGImage.bmp "C:\Documents and Settings\Administrator\����\{BA257683-364A-4EB7-8F37-20328F4D4268}.bmp"
  # BGImage::SetBG /NOUNLOAD /FILLSCREEN "$PLUGINSDIR\BGImage.bmp"
  # Delete "$PLUGINSDIR\BGImage.bmp"
  # SetOutPath "$PLUGINSDIR"
  # File /oname=BGBrandingImage.bmp "C:\Documents and Settings\Administrator\����\{EC761AE8-84D1-4B7C-9826-FC254F320008}.bmp"
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
  # MessageBox MB_OK "������Ϣ��mpw.mpk�ļ���װʧ��($INSTDIR\data\mpw.mpk ��С:$0 bytes,����)����ر�ɱ������ͷ���ǽ�����������°�װ��"
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
  # MessageBox MB_OK "������Ϣ��Creature.mpk�ļ���װʧ�ܣ���ر�ɱ������ͷ���ǽ�����������°�װ��"
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
  # MessageBox MB_OK "������Ϣ��maps.mpk�ļ���װʧ�ܣ���ر�ɱ������ͷ���ǽ�����������°�װ��"
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
  Exec '"$INSTDIR\Զ��Online.exe"'
FunctionEnd

# ===================================================================
# �ļ�ĩβ
# ===================================================================
