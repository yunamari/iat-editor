# -iat-editor
loads a dll and changes an imported function entry specified in the dynamically specified dll (with its rva) in the program whose pid was previously specified
# CLI
```
Usage: iat-editor <pid> <iat_rva> <dll_path> <func_name>

<pid>       : The PID of the target application
<iat_rva>   : The RVA (Relative Virtual Address) of the external function pointer you want to modify
<dll_path>  : The path of the DLL where the replacement function is located
<func_name> : The name of the replacement function (ensure this function is exported by the DLL)
```

