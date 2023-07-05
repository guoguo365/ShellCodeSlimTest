#include <Windows.h>
#include <iostream>

void _declspec(naked) ShellCode()
{
	_asm
	{
		push ebp
		mov ebp, esp
		sub esp, 0x4
		push ecx
		push edx
		push ebx

		mov dword ptr [ebp - 0x4], 0  // DWORD digest = 0

		mov esi, [ebp + 0x8]          // 取参数strName
		xor ecx, ecx
	tag_hash_loop:
		mov ebx, [ebp - 0x4]
		shl ebx, 0x19				  // digest << 25
		mov edx, [ebp - 0x4]
		shr edx, 0x7                  // digest >> 7
		or ebx, edx					  // |
		
		
		xor eax, eax
		mov al, [esi + ecx]           // strName是一个字节，所以要放在al中
		test al, al                   // al是否未0
		jz tag_hash_loop_end          
		add ebx, eax				  // digest + *strName
		mov [ebp - 0x4], ebx          // 保存结果

		inc ecx                       // strName++
		jmp tag_hash_loop
	tag_hash_loop_end:
		mov eax, [ebp - 0x4]

		pop ebx
		pop edx
		pop ecx
		mov esp, ebp
		pop ebp
		retn 0x4
	}
}

DWORD GetHashCode(char* strName)
{
	DWORD digest = 0;
	while (*strName)
	{
		digest = (digest << 25 | digest >> 7);
		digest = digest + *strName;
		strName++;
	}
	return digest;
}

char strName[] = "I Love you";

int main()
{
	
	printf("%s\n", strName);
	_asm
	{
		lea eax, strName
		push eax
		call ShellCode
	}

	DWORD dd = GetHashCode(strName);
	printf("%x\n", dd);
	system("pause");
	return 0;
}