// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <iostream>
#include <iomanip>
#include <windows.h>
#include <TlHelp32.h>
#include "mem.h"

DWORD APIENTRY appEntry(HMODULE hModule)
{
    // Create Console
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
 
    //--------------------------------------------

    // Module Base
    uintptr_t modBase = (uintptr_t)GetModuleHandle("CoDWaW.exe");
    //--------------------------------------------

    // Variables
    bool clearScreen = true;
    bool ammoOn = false, healthOn = false, rapidFire = false, speed = false;
    uintptr_t healthLoc = modBase + 0xF31F4;
    uintptr_t pointLoc = modBase + 0x14EF124;
    uintptr_t rapidInstruc = modBase + 0x20BCB;
    uintptr_t speedLoc = modBase + 0x1DD7984;
    uintptr_t instructAddr = modBase + 0x1E619;
    uintptr_t x = modBase + 0x14ED088;
    uintptr_t y = modBase + 0x14ED08C;
    uintptr_t z = modBase + 0x14ED090;

    //--------------------------------------------

    // Game Loop
    while (true)
    {
        // Screen clear
        if (clearScreen)
        {
            system("cls");
            std::cout << std::setw(75) << "COD WAW TRAINER V.2 \n";
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
        // Key inputs
        // Instead of Patching you can change values directly
        if (GetAsyncKeyState(VK_INSERT) & 1)
        {
            break;
        }
        if (GetAsyncKeyState(VK_NUMPAD1) & 1)
        {
            float currentLocX = *(float*)x;
            float currentLocY = *(float*)y;
            float currentLocZ = *(float*)z;
            
            float teleportX = 556;
            float teleportY = -1510;
            float teleportZ = 64;

            *(float*)x = 556;
            *(float*)y = -1510;
            *(float*)z = 64;

            Sleep(250);
            inputF();
            Sleep(4500);
            inputF();
            Sleep(1000);

            *(float*)x = currentLocX;
            *(float*)y = currentLocY;
            *(float*)z = currentLocZ;
            clearScreen = true;
        }
        if (GetAsyncKeyState(VK_NUMPAD2) & 1)
        {
            ammoOn = !ammoOn;
            if (ammoOn)
            {
                mem::Nop((BYTE*)instructAddr, 7);
            }
            else
            {
                mem::Patch((BYTE*)instructAddr, (BYTE*)"\x89\x84\x8F\xFC\x05\x00\x00", 7);
            }
            clearScreen = true;
        }
        if (GetAsyncKeyState(VK_NUMPAD3) & 1)
        {
            /* Instead of this
            int newPoints = 999999;
            mem::Patch((BYTE*)pointLoc, (BYTE*)newPoints, sizeof(int));
            */
            int* newPoints = (int*)pointLoc;
            *newPoints = 999999;
            clearScreen = true;
        }
        if (GetAsyncKeyState(VK_NUMPAD4) & 1)
        {
            float* hideX = (float*)x;
            float* hideY = (float*)y;
            float* hideZ = (float*)z;
            *hideX = 110;
            *hideY = -1225;
            *hideZ = 30;
            clearScreen = true;
        }
        if (GetAsyncKeyState(VK_NUMPAD5) & 1)
        {
            healthOn = !healthOn;
            if (healthOn)
            {
                mem::Nop((BYTE*)healthLoc, 6);
            }
            else
            {
                mem::Patch((BYTE*)healthLoc, (BYTE*)"\x89\x96\xC8\x01\x00\x00", 6);
            }
            clearScreen = true;
        }
        if (GetAsyncKeyState(VK_NUMPAD6) & 1)
        {
            rapidFire = !rapidFire;
            if (rapidFire)
            {
                mem::Nop((BYTE*)rapidInstruc, 5);
            }
            else
            {
                mem::Patch((BYTE*)rapidInstruc, (BYTE*)"\xE8\x10\xFA\xFF\xFF", 5);
            }
            clearScreen = true;
        }
        if (GetAsyncKeyState(VK_NUMPAD7) & 1)
        {
            speed = !speed;
            int* speedV = (int*)speedLoc;
            if (speed)
            {
                *speedV = 1000;
            }
            else
            {
                *speedV = 190;
            }
            clearScreen = true;
        }

        Sleep(80);
    }
    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)imIn, hModule, 0, nullptr));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

