#include "dll_ut.h"
#include <iostream>

bool Inject(DWORD processId, const std::string& dllPath) {
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (!hProcess) {
        std::cerr << "Error: Unable to open process with ID " << processId << ".\n";
        return false;
    }

    LPVOID pRemoteMemory = VirtualAllocEx(hProcess, NULL, dllPath.size() + 1, MEM_COMMIT, PAGE_READWRITE);
    if (!pRemoteMemory) {
        std::cerr << "Error: Unable to allocate memory in target process.\n";
        CloseHandle(hProcess);
        return false;
    }

    if (!WriteProcessMemory(hProcess, pRemoteMemory, dllPath.c_str(), dllPath.size() + 1, NULL)) {
        std::cerr << "Error: Unable to write to target process memory.\n";
        VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, pRemoteMemory, 0, NULL);
    if (!hThread) {
        std::cerr << "Error: Unable to create remote thread in target process.\n";
        VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    WaitForSingleObject(hThread, INFINITE);

    VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE);
    CloseHandle(hThread);
    CloseHandle(hProcess);

    return true;
}



bool GetFuncAddr(const std::string& dll_path, const std::string& func_name, uintptr_t& func_addr) {
    HMODULE mod = LoadLibraryA(dll_path.c_str());
    if (!mod) {
        std::cerr << "Error: Unable to load DLL " << dll_path << ".\n";
        return false;
    }

    FARPROC addr = GetProcAddress(mod, func_name.c_str());
    if (!addr) {
        std::cerr << "Error: Function " << func_name << " not found in " << dll_path << ".\n";
        FreeLibrary(mod);
        return false;
    }

    func_addr = reinterpret_cast<uintptr_t>(addr);
    FreeLibrary(mod);
    return true;
}
