# -iat-editor
loads a dll and changes an imported function entry specified in the dynamically specified dll (with its rva) in the program whose pid was previously specified
# CLI
```
Usage: iat-editor <pid> <iat_rva> <dll_path> <func_name>
```
`<pid>`       : the pid of the target application
`<iat_rva>`   : the rva of your external function pointer
`<dll_path>`  : the path of the dll in which your replacement function is located
`<func_name>` : the name of your replacement function (make sure that this function is exported by the dll)

