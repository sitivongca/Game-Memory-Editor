#include "patch.h"


// Give permission to edit byte, edit then give permission back
void bytePatchEx(BYTE* dst, BYTE* src, unsigned int size, HANDLE processHandle)
{
	DWORD temp;
	VirtualProtectEx(processHandle, dst, size, PAGE_EXECUTE_READWRITE, &temp);
	WriteProcessMemory(processHandle, dst, src, size, NULL);
	VirtualProtectEx(processHandle, dst, size, temp, &temp);
}


// Put in the dst, size and handle, size is for amount of bytes to nop
void byteNopEx(BYTE* dst, int size, HANDLE processHandle)
{
	BYTE* nop = new BYTE[size];
	memset(nop, 0x90, size);
	bytePatchEx(dst, nop, size, processHandle);
	delete[] nop;
}