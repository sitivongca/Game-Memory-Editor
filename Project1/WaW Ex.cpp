#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <iomanip>
#include "patch.h"
#include "procFind.h"


DWORD procId = findPID("CoDWaW.exe");
uintptr_t modBase = findModA(procId, L"CoDWaW.exe");
uintptr_t localEnt = modBase + 0xBE1C0;
HANDLE procHandle = OpenProcess(PROCESS_ALL_ACCESS, 0, procId);
uintptr_t healthLoc = modBase + 0xF31F4;
uintptr_t pointLoc = modBase + 0x14EF124;
uintptr_t rapidInstruc = modBase + 0x20BCB;
uintptr_t speedLoc = modBase + 0x1DD7984;
uintptr_t x = modBase + 0x14ED088;
uintptr_t y = modBase + 0x14ED08C;
uintptr_t z = modBase + 0x14ED090;

float currentLocX, currentLocY, currentLocZ;
float teleportX = 566, teleportY = -1510, teleportZ = 64;
float hideX = 110, hideY = -1225, hideZ = 30;
// std::vector<unsigned int> ammoVec = { 0x374, 0x14, 0 };
// uintptr_t ammoLoc = findOffset(procHandle, localEnt, ammoVec);
// int newHealth = 9999;

uintptr_t instructAddr = modBase + 0x1E619;
bool ammoOn = false, healthOn = false, rapidFire = false, speed = false;
bool clearScreen = true;

void getOriginalLoc()
{
    ReadProcessMemory(procHandle, (BYTE*)(modBase + 0x14ED088), &currentLocX, sizeof(currentLocX), NULL);
    ReadProcessMemory(procHandle, (BYTE*)(modBase + 0x14ED08C), &currentLocY, sizeof(currentLocY), NULL);
    ReadProcessMemory(procHandle, (BYTE*)(modBase + 0x14ED090), &currentLocZ, sizeof(currentLocZ), NULL);
}
void originTeleport()
{
    WriteProcessMemory(procHandle, (BYTE*)x, &currentLocX, sizeof(currentLocX), NULL);
    WriteProcessMemory(procHandle, (BYTE*)y, &currentLocY, sizeof(currentLocY), NULL);
    WriteProcessMemory(procHandle, (BYTE*)z, &currentLocZ, sizeof(currentLocZ), NULL);
}

void boxTeleport(uintptr_t x, uintptr_t y, uintptr_t z)
{
    WriteProcessMemory(procHandle, (BYTE*)x, &teleportX, sizeof(teleportX), NULL);
    WriteProcessMemory(procHandle, (BYTE*)y, &teleportY, sizeof(teleportY), NULL);
    WriteProcessMemory(procHandle, (BYTE*)z, &teleportZ, sizeof(teleportZ), NULL);
}

void inputF()
{
    // needed for set input
    INPUT input;
    // not sure
    memset(&input, 0, sizeof(input));
    input.type = INPUT_KEYBOARD;

    // This is asking for key code, vKKeyScan translates to the keycode
    input.ki.wVk = VkKeyScan('f');
    SendInput(1, &input, sizeof(INPUT));
    Sleep(500);
    input.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(INPUT));
}


int main()
{
    if (procId == 0)
    {
        std::cout << "Process not found.\n";
        return 0;

    }
    else
    {
        DWORD exitptr = 0;
        while (GetExitCodeProcess(procHandle, &exitptr) && exitptr == STILL_ACTIVE)
        {
            if (clearScreen)
            {
                system("cls");
                std::cout << std::setw(75) << "COD WAW TRAINER V.1 \n";
                std::cout << std::setw(81) << "======================================\n";

                std::cout << std::setw(51) << "[NUM 1] -" << " Spin the Box\n";

                std::cout << std::setw(68) << "[NUM 2] - Unlimited Ammo <" << (ammoOn ? "on" : "off") << ">\n";

                std::cout << std::setw(51) << "[NUM 3] -" << " Set Points to 999999\n";

                std::cout << std::setw(51) << "[NUM 4] -" << " To Hide\n";

                std::cout << std::setw(70) << "[NUM 5] - Unlimited Health <" << (healthOn ? "on" : "off") << ">\n";

                std::cout << std::setw(64) << "[NUM 6] - Triggerbot <" << (rapidFire ? "on" : "off") << ">\n";

                std::cout << std::setw(52) << "[NUM 7] - " << (speed ? "Press to Slow Down" : "Press to Go Fast") << "\n";

                std::cout << std::setw(52) << "[INSERT] -" << " Exit Trainer\n";
                clearScreen = false;
            }

            // Current bug, inputF() not pressing f
            if (GetAsyncKeyState(VK_NUMPAD1) && 1)
            {
                // save current location
                getOriginalLoc();
                //teleport to box
                boxTeleport(x, y, z);
                Sleep(250);
                inputF();
                Sleep(4500);
                inputF();
                Sleep(1000);
                originTeleport();
                // Hit box wait grab weapon and leave
            }
            if (GetAsyncKeyState(VK_NUMPAD2) && 1)
            {
                /*
                ammoOn = !ammoOn;
                if (ammoOn)
                {
                    DWORD temp;
                    VirtualProtectEx(procHandle, (BYTE*)(instructAddr), 7, PAGE_EXECUTE_READWRITE, &temp);
                    WriteProcessMemory(procHandle, (BYTE*)(instructAddr), "\x90\x90\x90\x90\x90\x90\x90", 7, NULL);
                    VirtualProtectEx(procHandle, (BYTE*)(instructAddr), 7, temp, &temp);
                    clearScreen = true;
                }
                else
                {
                    DWORD temp;
                    VirtualProtectEx(procHandle, (BYTE*)(instructAddr), 7, PAGE_EXECUTE_READWRITE, &temp);
                    WriteProcessMemory(procHandle, (BYTE*)(instructAddr), (BYTE*)"\x89\x84\x8F\xFC\x05\x00\x00", 7, NULL);
                    VirtualProtectEx(procHandle, (BYTE*)(instructAddr), 7, temp, &temp);
                    clearScreen = true;
                }
                
                
                Why does this one not work?
                It's only filling 4 bytes, not 7
                Because sizeof(replaceAmmo) = 4 for some reason not 7
                WriteProcessMemory(procHandle, (BYTE*)(instructAddr), replaceAmmo, sizeof(replaceAmmo), NULL);
                */
                ammoOn = !ammoOn;
                if (ammoOn)
                {
                    DWORD temp;
                    BYTE* replaceAmmo = new BYTE[7];
                    memset(replaceAmmo, 0x90, 7);
                    VirtualProtectEx(procHandle, (BYTE*)(instructAddr), 7, PAGE_EXECUTE_READWRITE, &temp);
                    WriteProcessMemory(procHandle, (BYTE*)(instructAddr), replaceAmmo, 7, NULL);
                    VirtualProtectEx(procHandle, (BYTE*)(instructAddr), 7, temp, &temp);
                    delete[] replaceAmmo;
                    clearScreen = true;
                }
                else
                {
                    DWORD temp;
                    VirtualProtectEx(procHandle, (BYTE*)(instructAddr), 7, PAGE_EXECUTE_READWRITE, &temp);
                    WriteProcessMemory(procHandle, (BYTE*)(instructAddr), (BYTE*)"\x89\x84\x8F\xFC\x05\x00\x00", 7, NULL);
                    VirtualProtectEx(procHandle, (BYTE*)(instructAddr), 7, temp, &temp);
                    clearScreen = true;
                }
            }
            if (GetAsyncKeyState(VK_NUMPAD3) && 1)
            {
                int newValue = 999999;
                WriteProcessMemory(procHandle, (BYTE*)(pointLoc), &newValue, sizeof(int), NULL);
            }
            if (GetAsyncKeyState(VK_NUMPAD4) && 1)
            {
                WriteProcessMemory(procHandle, (BYTE*)x, &hideX, sizeof(hideX), NULL);
                WriteProcessMemory(procHandle, (BYTE*)y, &hideY, sizeof(hideY), NULL);
                WriteProcessMemory(procHandle, (BYTE*)z, &hideZ, sizeof(hideZ), NULL);
            }

            if (GetAsyncKeyState(VK_NUMPAD5) && 1)
            {
                healthOn = !healthOn;
                if (healthOn)
                {
                    DWORD temp;
                    VirtualProtectEx(procHandle, (BYTE*)healthLoc, 6, PAGE_EXECUTE_READWRITE, &temp);
                    WriteProcessMemory(procHandle, (BYTE*)healthLoc, "\x90\x90\x90\x90\x90\x90", 6, NULL);
                    VirtualProtectEx(procHandle, (BYTE*)healthLoc, 6, temp, &temp);
                    clearScreen = true;
                }
                else 
                {
                    DWORD temp;
                    VirtualProtectEx(procHandle, (BYTE*)healthLoc, 6, PAGE_EXECUTE_READWRITE, &temp);
                    WriteProcessMemory(procHandle, (BYTE*)healthLoc, "\x89\x96\xC8\x01\x00\x00", 6, NULL);
                    VirtualProtectEx(procHandle, (BYTE*)healthLoc, 6, temp, &temp);
                    clearScreen = true;
                }
            }
            if (GetAsyncKeyState(VK_NUMPAD6) && 1)
            {
                rapidFire = !rapidFire;
                if (rapidFire)
                {
                    DWORD temp;
                    VirtualProtectEx(procHandle, (BYTE*)rapidInstruc, 5, PAGE_EXECUTE_READWRITE, &temp);
                    WriteProcessMemory(procHandle, (BYTE*)rapidInstruc, "\x90\x90\x90\x90\x90", 5, NULL);
                    VirtualProtectEx(procHandle, (BYTE*)rapidInstruc, 5, temp, &temp);
                    clearScreen = true;
                }
                else
                {
                    DWORD temp;
                    VirtualProtectEx(procHandle, (BYTE*)rapidInstruc, 5, PAGE_EXECUTE_READWRITE, &temp);
                    WriteProcessMemory(procHandle, (BYTE*)rapidInstruc, "\xE8\x10\xFA\xFF\xFF", 5, NULL);
                    VirtualProtectEx(procHandle, (BYTE*)rapidInstruc, 5, temp, &temp);
                    clearScreen = true;
                }
            }

            if (GetAsyncKeyState(VK_NUMPAD7) && 1)
            {
                speed = !speed;
                int newValue = 1000;
                if (speed)
                {
                    WriteProcessMemory(procHandle, (BYTE*)(speedLoc), &newValue, sizeof(int), NULL);
                    clearScreen = true;
                }
                else 
                {
                    newValue = 190;
                    WriteProcessMemory(procHandle, (BYTE*)(speedLoc), &newValue, sizeof(int), NULL);
                    clearScreen = true;
                }
            }

            if (GetAsyncKeyState(VK_INSERT) && 1)
            {
                return 0;
            }

            Sleep(100);
            
        }

    }
    CloseHandle(procHandle);
    return 0;
}
