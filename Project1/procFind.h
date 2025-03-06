#include <Windows.h>
#include <tlhelp32.h>
#include <vector>
#include <iostream>
#include <iomanip>
#ifndef PROCFIND_H
#define PROCFIND_H

#pragma once

// Only need process name
DWORD findPID(const char* procName);

// Need process name and ID
uintptr_t findModA(DWORD procId, const wchar_t* procName);

//Need handle to process, the module base, and how many offsets you want to move
uintptr_t findOffset(HANDLE procId, uintptr_t modBase, std::vector<unsigned int> offset);

#endif