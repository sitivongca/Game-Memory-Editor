#pragma once
#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>

namespace mem
{
	void Patch(BYTE* dst, BYTE* src, int size);

	void Nop(BYTE* dst, int size);
	
	void offsets(uintptr_t modBase, std::vector<unsigned int> offsets);
}

void inputF();
