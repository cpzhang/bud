#ifndef __bootstrapper_cpp__
#define __bootstrapper_cpp__

//
#include <iostream>

/*
Pentium���������ļĴ������ֻ࣬��8��32λͨ�üĴ������ֱ𱻳�ΪEAX, EBX, ECX, EDX, EBP, ESP, EDI , ESI��
ÿһ��ͨ�üĴ����ĵ�16λ�ֱַ𱻳�ΪAX, BX, CX, DX, BP, SP, DI , SI������AX, BX, CX, DX�ĸ�8λ����ΪAH, BH, CH, DH��
��8λ����ΪAL, BL, CL, DL��
ע������Ƕ����в�Ӧʹ��EBP��ESP�����Ǵ洢����Ҫ�Ķ�ջ��Ϣ��
����һ���ǳ���Ҫ�ļĴ�����������־�Ĵ���(EFLAGS)���������������ĸ������ԣ��㲻��ֱ�Ӷ�ȡ���޸�����
��Щ�����У������/���(OF)����/��(SF)������/��(ZF)��ż/��(PF)������λ/��λ(CF)�ȡ�
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
