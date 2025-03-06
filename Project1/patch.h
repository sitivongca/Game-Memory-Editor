#include <Windows.h>
#include <tlhelp32.h>
#include <vector>
#pragma once

// Arguments are where to, what bytes, how many, and in what process
void bytePatchEx(BYTE* dst, BYTE* src, unsigned int size, HANDLE processHandle);

// Same as PatchEx except 0x90 will replace src
void byteNopEx(BYTE* dst, int size, HANDLE processHandle);