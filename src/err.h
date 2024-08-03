#ifndef ERROR_UTILS_H
#define ERROR_UTILS_H

#include <string>
#include <Windows.h>

std::string ErrorMsg(DWORD err_code);
void PrintUsage();

#endif
