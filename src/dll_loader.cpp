#include "dll_ut.h"
#include "err.h"
#include <iostream>
#include "process_ut.h"



bool Inject(DWORD pid, const std::string& dll_path) {
    HANDLE hProcess = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_OPERATION | PROCESS_CREATE_THREAD, FALSE, pid);
    if (!hProcess) {
        std::cerr << "Error: Unable to open process with ID " << pid << " : " << ErrorMsg(GetLastError()) << std::endl;
        return false;
    }

 
    LPVOID dll_mem = VirtualAllocEx(hProcess, nullptr, dll_path.size() + 1, MEM_COMMIT, PAGE_READWRITE);
    if (!dll_mem) {
        std::cerr << "Error: Unable to allocate memory in target process : " << ErrorMsg(GetLastError()) << std::endl;
        CloseHandle(hProcess);
        return false;
    }


    if (!WriteProcessMemory(hProcess, dll_mem, dll_path.c_str(), dll_path.size() + 1, nullptr)) {
        std::cerr << "Unable to write to target process memory : " << ErrorMsg(GetLastError()) << std::endl;
        VirtualFreeEx(hProcess, dll_mem, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }


    HANDLE hth = CreateRemoteThread(hProcess, nullptr, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, dll_mem, 0, nullptr);
    if (!hth) {
        std::cerr << "Unable to create remote thread in target process : " << ErrorMsg(GetLastError()) << std::endl;
        VirtualFreeEx(hProcess, dll_mem, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }
 
    WaitForSingleObject(hth, INFINITE);
    VirtualFreeEx(hProcess, dll_mem, 0, MEM_RELEASE);
    CloseHandle(hth);
    CloseHandle(hProcess);
    return true;
}



bool GetFuncAddr(const std::string& dll_path, const std::string& func_name, uint64_t& func_addr) {
    HMODULE mod = LoadLibraryA(dll_path.c_str());
    if (!mod) {
        std::cerr << "Unable to load dll " << dll_path << " : " << ErrorMsg(GetLastError()) << std::endl;
        return false;
    }

    FARPROC addr = GetProcAddress(mod, func_name.c_str());
    if (!addr) {
        std::cerr << "Function " << func_name << " not found in " << dll_path << " : " << ErrorMsg(GetLastError()) << std::endl;
        FreeLibrary(mod);
        return false;
    }

    func_addr = (uint64_t)(addr);
    FreeLibrary(mod);
    return true;
}
