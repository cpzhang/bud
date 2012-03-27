# -------------------------------------------------------------------
# 取得mac地址，放进$6
# -------------------------------------------------------------------

# ===================================================================
# 外面调用
# ===================================================================
 Function GetMacAddress
   System::Call Iphlpapi::GetAdaptersInfo(i,*i.r0)
   System::Alloc $0
   Pop $1
   System::Call Iphlpapi::GetAdaptersInfo(ir1r2,*ir0)i.r0
   StrCmp $0 0 0 finish
 loop:
   StrCmp $2 0 finish
   System::Call '*$2(i.r2,i,&t260.s,&t132.s,i.r5)i.r0' ;Unicode版将t改为m
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
   Goto loop
 finish:
   System::Free $1
 FunctionEnd
