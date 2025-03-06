#include "mem.h"

// Functions stay in here because they don't work in dllmain

// I originally did uintptr_t but switched to BYTE* because you have to typecast anyway

// Even though you're internal you still need write permission
void mem::Patch(BYTE* dst, BYTE* src, int size)
{
	DWORD temp;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &temp);
	// Instead of writing to memory just use memcpy
	memcpy(dst, src, size);
	VirtualProtect(dst, size, temp, &temp);
}

void mem::Nop(BYTE* dst, int size)
{
	DWORD temp;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &temp);
	memset(dst, 0x90, size);
	VirtualProtect(dst, size, temp, &temp);
}

void mem::offsets(uintptr_t modBase, std::vector<unsigned int> offsets)
{
	// int holding value of modBase
	uintptr_t address = modBase;
	// readprocessmemory dereferenced for you, now you need to dereference yourself
	for (int i = 0; i < offsets.size(); ++i)
	{
		// typecast into a pointer to an int that holds a pointer address,
		// it's uintptr_t so you can add offset
		address = *(uintptr_t*)address;
		address = address + offsets[i];
	}
}

void inputF()
{
	INPUT input;
	memset(&input, 0, sizeof(input));
	input.type = INPUT_KEYBOARD;

	input.ki.wVk = VkKeyScan('f');
	SendInput(1, &input, sizeof(INPUT));
	Sleep(500);
	input.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(INPUT));
}