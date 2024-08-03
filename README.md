# iat-editor
changes the value of a pointer to a specified iat entry (with its rva) to point to a function specified in the user-specified dll
# CLI
```
Usage: iat-editor <pid> <iat_rva> <dll_path> <func_name>

<pid>       : The PID of the target application
<iat_rva>   : The RVA (Relative Virtual Address) of the external function pointer you want to modify
<dll_path>  : The path of the DLL where the replacement function is located
<func_name> : The name of the replacement function (ensure this function is exported by the DLL)
```

