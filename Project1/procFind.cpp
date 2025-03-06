#include "procFind.h"

	DWORD findPID(const char* procName)
	{
		HANDLE snapshot = 0;
		DWORD procId = 0;
		snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (snapshot != INVALID_HANDLE_VALUE)
		{
			PROCESSENTRY32 entryStruct;
			entryStruct.dwSize = sizeof(PROCESSENTRY32);
			if (Process32First(snapshot, &entryStruct))
			{
				do
				{
					if (!_stricmp(entryStruct.szExeFile, procName))
					{
						procId = entryStruct.th32ProcessID;
						break;
					}
				} while (Process32Next(snapshot, &entryStruct));
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
		CloseHandle(snapshot);
		return procId;
	}

	uintptr_t findModA(DWORD procId, const wchar_t* procName)
	{
		uintptr_t modBase = 0;
		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);

		if (snapshot != INVALID_HANDLE_VALUE)
		{
			MODULEENTRY32W entryStruct;
			entryStruct.dwSize = sizeof(MODULEENTRY32W);

			if (Module32FirstW(snapshot, &entryStruct))
			{
				do
				{
					if (!_wcsicmp(entryStruct.szModule, procName))
					{
						modBase = (uintptr_t)entryStruct.modBaseAddr;
						break;
					}
				} while (Module32NextW(snapshot, &entryStruct));
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
		CloseHandle(snapshot);
		return modBase;
	}

	uintptr_t findOffset(HANDLE procHandle, uintptr_t localEnt, std::vector<unsigned int> offset)
	{
		uintptr_t address = localEnt;
		for(int i = 0; i < offset.size(); ++i)
		{
			ReadProcessMemory(procHandle, (BYTE*)address, &address, sizeof(address), NULL);
			address += offset[i];
		}
		return address;
	}