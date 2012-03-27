# -------------------------------------------------------------------
# 用途：删选出剩余空间最大的硬盘
# -------------------------------------------------------------------

# -------------------------------------------------------------------
# 全局变量
# -------------------------------------------------------------------
# 最大盘
var largestDisk
# 最大盘空间（M）
var largestSize
# 筛选集合，只考虑这几个盘
var disksFilter

# -------------------------------------------------------------------
# 回调，检测硬盘大小
# -------------------------------------------------------------------
Function DiskCallBack
  # 取得剩余空间
  ${DriveSpace} $9 "/D=F /S=M" $R0
  # 字符串比较，筛选集合
  ${WordFind} $disksFilter $9 "+1{" $R1
  StrCmp $R1 $disksFilter notfound found
# 不在筛选集合里面，结束
notfound:
  Goto done

# 在筛选集合里面，再检查剩余空间大小
found:
  IntCmp $R0 $largestSize is5 lessthan5 morethan5
# 没有超过当前最大剩余空间，结束
is5:
  Goto done
lessthan5:
  Goto done
# 超过了当前最大剩余空间，更新全局变量，记录最大盘及其剩余空间
morethan5:
  StrCpy $largestSize $R0
  StrCpy $largestDisk $9
  Goto done
done:
  Push $0
FunctionEnd

# -------------------------------------------------------------------
# 遍历硬盘，外面调用
# -------------------------------------------------------------------
Function IterateDisks
  # 初始化变量
  StrCpy $largestSize 0
  StrCpy $largestDisk $PROGRAMFILES
  StrCpy $disksFilter "C:\D:\E:\F:\"
  # 遍历
  ${GetDrives} "HDD" "DiskCallBack"
  # 设置安装目录
  StrCpy $INSTDIR "$largestDisk${Game_CompanyName}\${Game_Name}"
FunctionEnd
