#ifndef DLL_UTILS_H
#define DLL_UTILS_H

#include <string>
#include <Windows.h>

bool Inject(DWORD processId, const std::string& dllPath);
bool GetFuncAddr(const std::string& dll_path, const std::string& func_name, uintptr_t& func_addr);

#endif
