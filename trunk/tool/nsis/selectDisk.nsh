# -------------------------------------------------------------------
# ��;��ɾѡ��ʣ��ռ�����Ӳ��
# -------------------------------------------------------------------

# -------------------------------------------------------------------
# ȫ�ֱ���
# -------------------------------------------------------------------
# �����
var largestDisk
# ����̿ռ䣨M��
var largestSize
# ɸѡ���ϣ�ֻ�����⼸����
var disksFilter

# -------------------------------------------------------------------
# �ص������Ӳ�̴�С
# -------------------------------------------------------------------
Function DiskCallBack
  # ȡ��ʣ��ռ�
  ${DriveSpace} $9 "/D=F /S=M" $R0
  # �ַ����Ƚϣ�ɸѡ����
  ${WordFind} $disksFilter $9 "+1{" $R1
  StrCmp $R1 $disksFilter notfound found
# ����ɸѡ�������棬����
notfound:
  Goto done

# ��ɸѡ�������棬�ټ��ʣ��ռ��С
found:
  IntCmp $R0 $largestSize is5 lessthan5 morethan5
# û�г�����ǰ���ʣ��ռ䣬����
is5:
  Goto done
lessthan5:
  Goto done
# �����˵�ǰ���ʣ��ռ䣬����ȫ�ֱ�������¼����̼���ʣ��ռ�
morethan5:
  StrCpy $largestSize $R0
  StrCpy $largestDisk $9
  Goto done
done:
  Push $0
FunctionEnd

# -------------------------------------------------------------------
# ����Ӳ�̣��������
# -------------------------------------------------------------------
Function IterateDisks
  # ��ʼ������
  StrCpy $largestSize 0
  StrCpy $largestDisk $PROGRAMFILES
  StrCpy $disksFilter "C:\D:\E:\F:\"
  # ����
  ${GetDrives} "HDD" "DiskCallBack"
  # ���ð�װĿ¼
  StrCpy $INSTDIR "$largestDisk${Game_CompanyName}\${Game_Name}"
FunctionEnd
