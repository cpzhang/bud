# -------------------------------------------------------------------
# ��;������ļ���С�Ƿ����𻵣����Լ�⣬����ȷ
# -------------------------------------------------------------------

# -------------------------------------------------------------------
# ȫ�ֱ���
# -------------------------------------------------------------------
# �������ļ���
var _fileName
# ���ļ���С��С����
var _size

# -------------------------------------------------------------------
# ���ĳ�����ļ��Ĵ�С���������
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
# ���ʵ��
# -------------------------------------------------------------------
Function checkImp
  # ȡ���ļ���С
  Push $_fileName
  Exch $0
  Push $1
  FileOpen $1 $0 "r"
  FileSeek $1 0 END $0
  FileSeek $1 0 END $2
  FileClose $1
  Pop $1
  Exch $0

  # �Ƚϴ�С 
  IntCmpU $2 $_size equal less greater
equal:
  Goto done
less:
  MessageBox MB_OK "������Ϣ���ļ�����)����ر�ɱ������ͷ���ǽ�����������°�װ��"
  Goto done
greater:
  Goto done
done:
FunctionEnd
