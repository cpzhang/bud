#ifndef __bootstrapper_cpp__
#define __bootstrapper_cpp__

//
#include <iostream>

/*
Pentium级处理器的寄存器不多，只有8个32位通用寄存器，分别被称为EAX, EBX, ECX, EDX, EBP, ESP, EDI , ESI。
每一个通用寄存器的低16位又分别被称为AX, BX, CX, DX, BP, SP, DI , SI。其中AX, BX, CX, DX的高8位被称为AH, BH, CH, DH；
低8位被称为AL, BL, CL, DL。
注意在内嵌汇编中不应使用EBP和ESP，它们存储着重要的堆栈信息。
还有一个非常重要的寄存器，叫做标志寄存器(EFLAGS)，标明了运算结果的各个属性，你不能直接读取或修改它。
这些属性有：不溢出/溢出(OF)、正/负(SF)、非零/零(ZF)、偶/奇(PF)、不进位/进位(CF)等。
*/
// Entry Point
int main()
{
	unsigned int a, b;
	std::cin>>a>>b;
	unsigned int* c = &a;
	__asm
	{
		mov eax, c;
		mov eax, [eax];
		mov ebx, b;
		lea eax, [eax + ebx];
		mov a, eax
	}
	std::cout<<a;
}

#endif // __bootstrapper_cpp__
