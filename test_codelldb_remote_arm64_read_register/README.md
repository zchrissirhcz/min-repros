# test codelldb remote debug arm64 read register
For https://github.com/vadimcn/vscode-lldb/issues/708

## VSCode CodeLLDB remote debug output
![](codelldb_lldb_prompt.png)

As shown above, the command `register read x0` is not working.

## LLDB remote debug output
This is the original LLDB command remote debug output, i.e. the expected output:

![](lldb_remote_debug_output.png)