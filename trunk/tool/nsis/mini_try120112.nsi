# ͷ�ļ�
!include "FileFunc.nsh"
!include "WordFunc.nsh"
!include "MUI.nsh"

# -------------------------------------------------------------------
# ���ù���Ŀ¼�������õ��������Դ������ͼƬ��������Ϣ
# -------------------------------------------------------------------
!cd "C:\Program Files\NSIS\Resources"


# -------------------------------------------------------------------
# ȫ�ֱ���
# -------------------------------------------------------------------
var EnterDescription
var Version_Alias

# -------------------------------------------------------------------
# ѹ��
# -------------------------------------------------------------------
# SetCompressor /SOLID LZMA
SetCompressor LZMA
SetCompressorDictSize 32
SetCompress Auto
SetDatablockOptimize On

# -------------------------------------------------------------------
# ����
# -------------------------------------------------------------------
# ��˾����
!define Game_CompanyName "��������"
# ��ʼ�˵��ļ���Ŀ¼
!define STARTMENU_FOLDER "��������"
# ��װ����ı���
!define Game_Title "Զ��Online"
# Ĭ�ϰ�װĿ¼
!define Game_InstallDir "$PROGRAMFILES\��������\Զ��Online"
# ���ɵİ�װ�����ļ�
!define Game_OutFile "f:\setup\game_patch.exe"
# ��װ�����ͼ��
!define Game_IconFile "y"
# �����ͼ��
!define Game_HeaderImage "yz12"
# �򵼵�ͼ�� 
!define Game_WizardImage "mm"
# ��Ϸ����
!define Game_Name "Զ��Online"
# -------------------------------------------------------------------
# Ԥ���ļ�
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
# ��������
# -------------------------------------------------------------------
XPStyle On
CRCCheck off
ShowInstDetails Show
ShowUninstDetails Show
BrandingText "Copyright(C) 2009��${Game_CompanyName}"
Name "${Game_Title}"
OutFile "${Game_OutFile}"
InstallDirRegKey HKCU "Software\��������\${Game_Name}" "Path"

# -------------------------------------------------------------------
# ��������
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
!insertmacro MUI_PAGE_LICENSE "C:\Program Files\NSIS\license_mini.txt"
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
# ��������
# -------------------------------------------------------------------
!insertmacro MUI_LANGUAGE "SimpChinese"
# �����
var largestDisk
# ����̿ռ䣨M��
var largestSize
# ֻ�����⼸����
var disksFilter
Function Example2
	# MessageBox MB_OK "$9  ($8 Drive)"
${DriveSpace} $9 "/D=F /S=M" $R0
	# MessageBox MB_OK $R0
${WordFind} $disksFilter $9 "+1{" $R1
StrCmp $R1 $disksFilter notfound found
	notfound:
	# MessageBox MB_OK 'Not found'
  Goto done
	found:
	# MessageBox MB_OK 'Found'

IntCmp $R0 $largestSize is5 lessthan5 morethan5
is5:
# MessageBox MB_OK "$R0 == $largestSize"
  Goto done
lessthan5:
# MessageBox MB_OK "$R0 < $largestSize"
  Goto done
morethan5:
# MessageBox MB_OK "$R0 > $largestSize $largestDisk"
StrCpy $largestSize $R0
  StrCpy $largestDisk $9
  Goto done
done:
	Push $0
FunctionEnd
Function IterateDisks
  StrCpy $largestSize 0
  StrCpy $largestDisk $PROGRAMFILES
  StrCpy $disksFilter "C:\D:\E:\F:\"
${GetDrives} "HDD" "Example2"
# MessageBox MB_OK "$largestSize $largestDisk"
StrCpy $INSTDIR "$largestDisk��������\Զ��Online"
FunctionEnd

# Section
# Call IterateDisks
# SectionEnd
# -------------------------------------------------------------------
# ��װ�ļ����� subst i: .
# -------------------------------------------------------------------
Section
  SetOutPath "$INSTDIR"
    File /r "f:\plato\bud\trunk\tool\nsis\data"
    # File /r "I:\setup_wei\updatescheme.xml"
    # File /r "I:\setup_wei\Զ��Online.exe"
    # File /r "I:\setup_wei\serverlist.xml"
    # File /r "I:\setup_wei\Skin"
SectionEnd


# -------------------------------------------------------------------
# ����ж�س�������
# -------------------------------------------------------------------
Section
  SetOutPath "$INSTDIR"

  WriteUninstaller "$INSTDIR\Uninstall.exe"
  # WriteRegStr HKCU "Software\${Game_CompanyName}\${Game_Name}"  "DisplayName" "${Game_Title}"
  # WriteRegStr HKCU "Software\${Game_CompanyName}\${Game_Name}" "DisplayIcon" "$\"$INSTDIR\Uninstall.exe$\""
  # WriteRegStr HKCU "Software\${Game_CompanyName}\${Game_Name}" "UninstallString" "$\"$INSTDIR\Uninstall.exe$\""
  # WriteRegDWORD HKCU "Software\${Game_CompanyName}\${Game_Name}" "NoModify" 1
  # WriteRegDWORD HKCU "Software\${Game_CompanyName}\${Game_Name}" "NoRepair" 1
SectionEnd


# -------------------------------------------------------------------
# ��ʼ�˵�����
# -------------------------------------------------------------------
Section
  # !insertmacro MUI_STARTMENU_WRITE_BEGIN Application

  SetShellVarContext all
  CreateDirectory "$SMPROGRAMS\${STARTMENU_FOLDER}"
  CreateShortCut "$SMPROGRAMS\${STARTMENU_FOLDER}\${Game_Title}.lnk" "$INSTDIR\${Game_Title}.exe"
  CreateShortCut "$SMPROGRAMS\${STARTMENU_FOLDER}\$(^UninstallCaption).lnk" "$INSTDIR\Uninstall.exe"
  SetShellVarContext current

  # !insertmacro MUI_STARTMENU_WRITE_END
SectionEnd


# -------------------------------------------------------------------
# ��������
# -------------------------------------------------------------------
Section
  SetShellVarContext all
  CreateShortCut "$DESKTOP\${Game_Name}.lnk" "$INSTDIR\${Game_Name}.exe"
  SetShellVarContext current
SectionEnd


# -------------------------------------------------------------------
# ע������
# -------------------------------------------------------------------
Section
  # WriteRegStr HKCU "Software\��������\${Game_Name}" "" "${Game_Title}"
  WriteRegStr HKCU "Software\${Game_CompanyName}\${Game_Name}" "Path" "$INSTDIR"
SectionEnd


# -------------------------------------------------------------------
# ��ʼ������
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
  MessageBox MB_OK|MB_ICONSTOP|MB_TOPMOST "�������û� $\"$0$\" ��װ���Ӧ�ó���.$\n�������Ĺ���Ա������� !!!"
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
# ж�س�������
# -------------------------------------------------------------------
Section "Uninstall"
  RMDir /r "$INSTDIR"

  # !insertmacro MUI_STARTMENU_GETFOLDER Application $R0
  SetShellVarContext all
  # RMDir /r "$SMPROGRAMS\$R0"
  RMDir /r "$SMPROGRAMS\${STARTMENU_FOLDER}"
  SetShellVarContext current

  SetShellVarContext all
  Delete "$DESKTOP\${Game_Name}.lnk"
  SetShellVarContext current

  DeleteRegKey HKCU "Software\${Game_CompanyName}\${Game_Name}"

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
  CreateFont $R0 "Verdana" 22 700
  BGImage::AddText /NOUNLOAD "$(^SetupCaption)" $R0 0xFF 0xFB 0xF0 10 10 -10 -10
  BGImage::Redraw /NOUNLOAD
FunctionEnd

Function un.onUninstSuccess
  BGImage::Destroy
  ExecShell "open" "http://api.yz.szgla.com/GameInstall/GameUnInstall.aspx$EnterDescription"
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
Var "MacAddress"

 Function .GetMacAddress
   System::Call Iphlpapi::GetAdaptersInfo(i,*i.r0)
   System::Alloc $0
   Pop $1
   System::Call Iphlpapi::GetAdaptersInfo(ir1r2,*ir0)i.r0
   StrCmp $0 0 0 finish
 loop:
   StrCmp $2 0 finish
   System::Call '*$2(i.r2,i,&t260.s,&t132.s,i.r5)i.r0' ;Unicode�潫t��Ϊm
   IntOp $3 403 + $5
   StrCpy $6 ""
   ${For} $4 404 $3
     IntOp $7 $0 + $4
     System::Call '*$7(&i1.r7)'
     IntFmt $7 "%02X" $7
     StrCpy $6 "$6$7"
     StrCmp $4 $3 +2
     StrCpy $6 "$6-"
   ${Next}
   StrCpy $MacAddress $6
   Goto loop
 finish:
   System::Free $1
 FunctionEnd
 
Function .onInstSuccess
  # Call FileCorrupt1
  # Call FileCorrupt2
  # Call FileCorrupt3
 Call .GetMacAddress
# MessageBox MB_OK "Mac=$MacAddress"
 # ExecShell "open" "http://api.yz.szgla.com/GameInstall/Install.aspx?GameName=yz3&Mac=$MacAddress&Remark=wei" 0 SW_SHOWMINIMIZED
  # ExecShell "" "$INSTDIR\Զ��Online.exe"
FunctionEnd
Function LaunchLink
	# MessageBox MB_OK "LaunchLink"
  ExecShell "open" "$INSTDIR\Զ��Online.exe" 0 SW_SHOWMINIMIZED
FunctionEnd
# ===================================================================
# �ļ�ĩβ
# ===================================================================
