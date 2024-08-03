#include "process_ut.h"
#include "err.h"
#include <psapi.h>
#include <iostream>

bool GetBaseAddr(DWORD pid, uintptr_t& base_addr) {
    HANDLE proc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (!proc) {
        std::cerr << "Unable to open process with pid " << pid << " : " << ErrorMsg(GetLastError()) << std::endl;
        return false;
    }

    HMODULE mods[1024];
    DWORD cb_needed;
    if (!EnumProcessModules(proc, mods, sizeof(mods), &cb_needed)) {
        std::cerr << "Unable to enumerate modules : " << ErrorMsg(GetLastError()) << std::endl;
        CloseHandle(proc);
        return false;
    }

    MODULEINFO mod_info;
    if (!GetModuleInformation(proc, mods[0], &mod_info, sizeof(mod_info))) {
        std::cerr << "Unable to get module information : " << ErrorMsg(GetLastError()) << std::endl;
        CloseHandle(proc);
        return false;
    }

    base_addr = (uint64_t)mod_info.lpBaseOfDll;
    CloseHandle(proc);
    return true;
}

bool Is64Proc(HANDLE proc) {
    BOOL is_wow64 = FALSE;
    if (IsWow64Process(proc, &is_wow64))
        return !is_wow64;
    return true;
}

bool Wpm(DWORD pid, LPVOID addr, LPVOID data) {
    HANDLE proc = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_OPERATION | PROCESS_QUERY_INFORMATION, FALSE, pid);
    if (!proc) {
        std::cerr << "Unable to open process with pid " << pid << " :" << ErrorMsg(GetLastError()) << std::endl;
        return false;
    }
    SIZE_T data_size = Is64Proc(proc) ? 8 : 4;
    DWORD old_protect;
    if (!VirtualProtectEx(proc, addr, data_size, PAGE_READWRITE, &old_protect)) {
        std::cerr << "Unable to change memory protection : " << ErrorMsg(GetLastError()) << std::endl;
        CloseHandle(proc);
        return false;
    }
    SIZE_T bytes_wrt = 0;
    bool res = WriteProcessMemory(proc, addr, data, data_size, &bytes_wrt);
    if (!VirtualProtectEx(proc, addr, data_size, old_protect, &old_protect)) 
        std::cerr << "Failed to restore memory protection at address " << addr << " : " << ErrorMsg(GetLastError()) << std::endl;
    
    CloseHandle(proc);
    if (!res || bytes_wrt != data_size) {
        std::cerr << "Unable to write to target process memory at " << std::hex << (uint64_t)addr << std::dec << " : " << ErrorMsg(GetLastError()) << std::endl;
        return false;
    }
    return true;
}
