## VS Code Configuration

Below is an example configuration for `.vscode/tasks.json` to enable building C files with GCC directly in VS Code. This task sets the include path to the project root and places the compiled executable in the same directory as the source file.

```jsonc
// Example .vscode/tasks.json snippet highlighting key settings
{
    "tasks": [
        {
            // ... other task settings ...
            "args": [
                // ... other args ...
                "-g", // Enable debugging symbols
                "-I${workspaceFolder}", // Include project root for headers like c1.h
                "${file}", // The file to compile
                "-o", // Specify output file
                "${fileDirname}/${fileBasenameNoExtension}" // Output executable next to source
            ],
            "options": {
                "cwd": "${fileDirname}" // Working directory is the source file's directory
            },
            // ... other task settings ...
        }
    ],
    "version": "2.0.0"
}
```
