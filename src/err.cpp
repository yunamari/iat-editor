#include "err.h"
#include <iostream>


std::string ErrorMsg(DWORD err_code) {
    LPVOID msg_buf;
    DWORD flags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
    FormatMessage(flags, NULL, err_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&msg_buf, 0, NULL);
    std::string err_msg((LPSTR)msg_buf);
    LocalFree(msg_buf);
    return err_msg;
}
