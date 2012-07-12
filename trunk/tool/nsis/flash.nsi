!include "FileFunc.nsh"
!include "WordFunc.nsh"

# -------------------------------------------------------------------
# ���峣��
# -------------------------------------------------------------------
!define STARTMENU_FOLDER "��������";
!define SFX_Title "Զ��Online[����]"; ��װ����ı���
!define SFX_InstallDir "$PROGRAMFILES\��������"; *Ĭ��* ��Ŀ�갲װĿ¼
!define SFX_OutFile "d:\rocketJX\Bin\setup.exe"; ���ɵİ�װ�����ļ�
!define SFX_BCNPP "BCNPP"

# -------------------------------------------------------------------
# ѡ��ѹ����
# -------------------------------------------------------------------
# SetCompressor /SOLID LZMA
SetCompressor LZMA
SetCompressorDictSize 32
SetCompress Auto
SetDatablockOptimize On
# ��Ĭ��װ
SilentInstall silent
SilentUnInstall silent

# -------------------------------------------------------------------
# ��������
# -------------------------------------------------------------------
XPStyle On
CRCCheck off
ShowInstDetails Show
ShowUninstDetails Show
BrandingText "Copyright(C) 2012����������"
Name "${SFX_Title}"
OutFile "${SFX_OutFile}"
InstallDir "${SFX_InstallDir}"

# -------------------------------------------------------------------
# ��װ�ļ�����
# -------------------------------------------------------------------
Section
  SetOutPath "$INSTDIR"
  File "d:\rocketJX\Bin\${SFX_BCNPP}.dll"
  File "d:\rocketJX\Bin\BCGAX.ocx"

  WriteUninstaller "$INSTDIR\Uninstall.exe"
  WriteRegStr HKCU "Software\MozillaPlugins\@szgla.com" "Description" "${SFX_BCNPP}"
  WriteRegStr HKCU "Software\MozillaPlugins\@szgla.com" "Name" "${SFX_BCNPP}"
  WriteRegStr HKCU "Software\MozillaPlugins\@szgla.com" "ProductName" "${SFX_BCNPP}"
  WriteRegStr HKCU "Software\MozillaPlugins\@szgla.com" "Path" "$INSTDIR\${SFX_BCNPP}.dll"

  Exec "regsvr32 /s $INSTDIR\BCGameActiveX.ocx"
SectionEnd


# -------------------------------------------------------------------
# ��ʼ������
# -------------------------------------------------------------------
Function .onInit
FunctionEnd

Function .onGUIEnd
  BGImage::Destroy
FunctionEnd

# -------------------------------------------------------------------
# ж�س�������
# -------------------------------------------------------------------
Section "Uninstall"
  Exec "regsvr32 /u $INSTDIR\BCGameActiveX.ocx"
  RMDir /r "$INSTDIR"
  DeleteRegKey HKCU "Software\MozillaPlugins\@szgla.com"
SectionEnd

# -------------------------------------------------------------------
# ж�س����ʼ������
# -------------------------------------------------------------------
Function un.onInit
FunctionEnd

Function un.onUninstSuccess
FunctionEnd

Function .onInstSuccess
  MessageBox MB_OK "Զ��OLҳ�β���Ѱ�װ�ɹ�����ˢ��ҳ��"
FunctionEnd

# ===================================================================
# �ļ�ĩβ
# ===================================================================
