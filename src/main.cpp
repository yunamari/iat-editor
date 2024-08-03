#include <iostream>
#include "process_ut.h"
#include "dll_ut.h"
#include "err.h"


int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cout << "Usage: iat-editor <pid> <iat_rva> <dll_path> <func_name>\n\n";
        std::cout << "<pid>       : The PID of the target application\n";
        std::cout << "<iat_rva>   : The RVA (Relative Virtual Address) of the external function pointer you want to modify\n";
        std::cout << "<dll_path>  : The path of the DLL where the replacement function is located\n";
        std::cout << "<func_name> : The name of the replacement function (ensure this function is exported by the DLL)" << std::endl;
        return 1;
    }
    DWORD pid = std::stoul(argv[1]);
    std::string iat_rva_str = argv[2];
    std::string dll_path = argv[3];
    std::string func_name = argv[4];

    uint64_t iat_rva = std::stoull(iat_rva_str, nullptr, 16);
    uint64_t base_addr = 0;

    if (!GetBaseAddr(pid, base_addr)) {
        std::cerr << "Failed to get base address of the process : " << ErrorMsg(GetLastError()) << std::endl;
        return 1;
    }

    if (!Inject(pid, dll_path)) {
        std::cerr << "Failed to inject dll " << ErrorMsg(GetLastError()) << std::endl;
        return 1;
    }
    std::cout << "the dll has been injected successfully" << std::endl;

    uint64_t func_addr = 0;
    if (!GetFuncAddr(dll_path, func_name, func_addr)) {
        std::cerr << "Failed to get address of the function: " << func_name << std::endl;
        return 1;
    }
    std::cout << "Base address of the target process: 0x" << std::hex << base_addr << std::dec << std::endl;
    std::cout << "Address of the function " << func_name << ": 0x" << std::hex << func_addr << std::dec << std::endl;
    uint64_t target_addr = base_addr + iat_rva;
    if (Wpm(pid, (LPVOID)target_addr, &func_addr)) 
       std::cout << "Successfully wrote to address 0x" << std::hex << target_addr << std::dec << std::endl;
    else 
       std::cerr << "Failed to write data to process memory at address 0x" << std::hex << target_addr << std::dec << std::endl;

    return 0;
}
