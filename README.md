# Linux-Kernel-Filesystem-Hook
The kernel module I developed allows for hooking file system operations such as open, read, write, and close. Additionally, the module accepts a path parameter to specify a specific directory to monitor for access.
This module hooks into the original system call functions by modifying the read-only flag and assigning custom functions to handle these operations.
