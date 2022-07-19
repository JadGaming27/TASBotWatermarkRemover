// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <windows.h>
#include <TlHelp32.h>
#include <string>
#include <iostream>

//bool show = true;
//const int addr = 0;
//
//void RenderMain() {
//    if (show) {
//        ImGui::Begin("TASBotMarkRemove");
//        ImGui::InputInt("Address", (int*)&addr, 1, 100, ImGuiInputTextFlags_CallbackEdit);
//        if (ImGui::Button("Disable Watermark", ImVec2(7, 2)))
//        {
//            std::string str;
//            SIZE_T bW;
//            char* res;
//            ReadProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(GetModuleBaseAddress(GetProcessId(GetCurrentProcess()), L"tBot.dll") + addr), (LPVOID)str.c_str(), str.length(), &bW);
//            memcpy(res, (void*)bW, sizeof bW);
//            DisableWatermark(addr);
//        }
//    }
//}

DWORD WINAPI my_thread(void* hModule) {
    //Your code goes here
    //====================
    /*ImGuiHook::setRenderFunction(RenderMain);
    ImGuiHook::setToggleCallback([]() {
        show = !show;
    });*/
    if (MH_Initialize() != MH_OK)
    {
        //This line will deattach your DLL when executed. Remove if needed
        FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(hModule), 0);
    } /*else {
        ImGuiHook::setupHooks([](void* target, void* hook, void** trampoline) { 
            MH_CreateHook(target, hook, trampoline);
        });
        MH_EnableHook(MH_ALL_HOOKS);
    }*/
    
    std::string str = "                ";
    DWORD ProcId = GetProcessId(GetCurrentProcess());
    SIZE_T bToW = str.length();
    SIZE_T bWrtn;
    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(GetModuleBaseAddress(ProcId, L"tBot.dll") + 0x24B568), (LPCVOID)str.c_str(), bToW, &bWrtn);
    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(GetModuleBaseAddress(ProcId, L"tBot.dll") + 0x24B59C), (LPCVOID)str.c_str(), bToW, &bWrtn);
}

uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName)
{
    uintptr_t modBaseAddr = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
    if (hSnap != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32 modEntry;
        modEntry.dwSize = sizeof(modEntry);
        if (Module32First(hSnap, &modEntry))
        {
            do
            {
                if (!_wcsicmp(modEntry.szModule, modName))
                {
                    modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
                    break;
                }
            } while (Module32Next(hSnap, &modEntry));
        }
    }
    CloseHandle(hSnap);
    return modBaseAddr;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0x1000, my_thread, hModule, 0, 0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

