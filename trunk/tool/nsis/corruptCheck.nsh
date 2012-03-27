# -------------------------------------------------------------------
# 用途：检测文件大小是否有损坏，粗略检测，不精确
# -------------------------------------------------------------------

# -------------------------------------------------------------------
# 全局变量
# -------------------------------------------------------------------
# 待检测的文件名
var _fileName
# 比文件大小稍小的数
var _size

# -------------------------------------------------------------------
# 检测某几个文件的大小，外面调用
# -------------------------------------------------------------------
Function CorruptCheck
  StrCpy $_fileName "$INSTDIR\data\mpw.mpk"
  StrCpy $_size 758438713
  Call checkImp

  StrCpy $_fileName  "$INSTDIR\data\Creature.mpk"
  StrCpy $_size 250965054
  Call checkImp

  StrCpy $_fileName  "$INSTDIR\data\maps.mpk"
  StrCpy $_size 180496434
  Call checkImp
FunctionEnd

# -------------------------------------------------------------------
# 检测实现
# -------------------------------------------------------------------
Function checkImp
  # 取得文件大小
  Push $_fileName
  Exch $0
  Push $1
  FileOpen $1 $0 "r"
  FileSeek $1 0 END $0
  FileSeek $1 0 END $2
  FileClose $1
  Pop $1
  Exch $0

  # 比较大小 
  IntCmpU $2 $_size equal less greater
equal:
  Goto done
less:
  MessageBox MB_OK "错误信息：文件已损坏)，请关闭杀毒软件和防火墙，并尝试重新安装！"
  Goto done
greater:
  Goto done
done:
FunctionEnd
