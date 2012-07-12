!include "FileFunc.nsh"
!include "WordFunc.nsh"

# -------------------------------------------------------------------
# 定义常量
# -------------------------------------------------------------------
!define STARTMENU_FOLDER "冰川网络";
!define SFX_Title "远征Online[极速]"; 安装程序的标题
!define SFX_InstallDir "$PROGRAMFILES\冰川网络"; *默认* 的目标安装目录
!define SFX_OutFile "d:\rocketJX\Bin\setup.exe"; 生成的安装程序文件
!define SFX_BCNPP "BCNPP"

# -------------------------------------------------------------------
# 选择压缩器
# -------------------------------------------------------------------
# SetCompressor /SOLID LZMA
SetCompressor LZMA
SetCompressorDictSize 32
SetCompress Auto
SetDatablockOptimize On
# 静默安装
SilentInstall silent
SilentUnInstall silent

# -------------------------------------------------------------------
# 常规设置
# -------------------------------------------------------------------
XPStyle On
CRCCheck off
ShowInstDetails Show
ShowUninstDetails Show
BrandingText "Copyright(C) 2012，冰川网络"
Name "${SFX_Title}"
OutFile "${SFX_OutFile}"
InstallDir "${SFX_InstallDir}"

# -------------------------------------------------------------------
# 安装文件区段
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
# 初始化函数
# -------------------------------------------------------------------
Function .onInit
FunctionEnd

Function .onGUIEnd
  BGImage::Destroy
FunctionEnd

# -------------------------------------------------------------------
# 卸载程序区段
# -------------------------------------------------------------------
Section "Uninstall"
  Exec "regsvr32 /u $INSTDIR\BCGameActiveX.ocx"
  RMDir /r "$INSTDIR"
  DeleteRegKey HKCU "Software\MozillaPlugins\@szgla.com"
SectionEnd

# -------------------------------------------------------------------
# 卸载程序初始化函数
# -------------------------------------------------------------------
Function un.onInit
FunctionEnd

Function un.onUninstSuccess
FunctionEnd

Function .onInstSuccess
  MessageBox MB_OK "远征OL页游插件已安装成功，请刷新页面"
FunctionEnd

# ===================================================================
# 文件末尾
# ===================================================================
