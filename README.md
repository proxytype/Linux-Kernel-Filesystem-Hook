# Linux-Kernel-Filesystem-Hook
The kernel module allows for hooking file system operations like open, read, write, and close. It can also accept a path parameter to specify a specific directory for monitoring access. The module hooks into the original system call functions by modifying the read-only flag and assigning custom functions to handle these operations.

# Usage:
compile using makefile and load the module.

## Load module with path to lookup
```
insmod bin/drv2.ko path=/tmp
```

## Remove module
```
rmmod drv2
```

Use demesg to see modules outputs.
