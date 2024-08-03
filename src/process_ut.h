#ifndef PROCESS_UTILS_H
#define PROCESS_UTILS_H

#include <Windows.h>
#include <string>

bool GetBaseAddr(DWORD pid, uintptr_t& base_addr);
bool Is64Proc(HANDLE proc);
bool Wpm(DWORD pid, LPVOID addr, LPVOID data);

#endif
