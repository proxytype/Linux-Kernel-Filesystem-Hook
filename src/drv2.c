#include "drv2.h"

static asmlinkage long localClose(const struct pt_regs *regs) 
{
    return old_close(regs);
}

static asmlinkage long localWrite(const struct pt_regs *regs) 
{
    return old_write(regs);
}

static asmlinkage long localRead(const struct pt_regs *regs) 
{
    return old_read(regs);
}

static asmlinkage long localOpenAt(const struct pt_regs *regs)
{
    const char __user *args_file = (const char __user *)regs->si;
    char user_filename[256] = {0};
    long copied = strncpy_from_user(user_filename, args_file, sizeof(user_filename));
    if (copied > 0){
        
        user_filename[sizeof(user_filename) - 1] = '\0';

        int result = strncmp(user_filename, path_lookup, strlen(path_lookup));
        if(result == 0) {
            pid_t pid = current->pid;
            printk(KERN_INFO "%s filename:%s pid:%d\n",__func__, user_filename, pid);
        }
    }
        
    return old_openat(regs);
}

void setRW(void)
{
    unsigned long value;
    asm volatile("mov %%cr0,%0" : "=r" (value));
    if (value & READ_ONLY_MASK)
    {
        value = value & ~READ_ONLY_MASK;
        asm volatile("mov %0,%%cr0": : "r" (value));
    }
}

void setRO(void)
{
    unsigned long value;
    asm volatile("mov %%cr0,%0" : "=r" (value));
    if (!(value & READ_ONLY_MASK))
    {
        value = value | READ_ONLY_MASK;
        asm volatile("mov %0,%%cr0": : "r" (value));
    }
}

static int __init init(void)
{
    sys_call_table = (void *)kallsyms_lookup_name(SYS_CALL_TABLE);
    
    printk("[info] %s. collect functions pointers \n", __func__);

    old_openat = sys_call_table[__NR_openat];
    old_read = sys_call_table[__NR_read];
    old_write = sys_call_table[__NR_write];
    old_close = sys_call_table[__NR_close];

    setRW();
    printk("[info] %s. overwrite functions pointers with new functions \n", __func__);
    sys_call_table[__NR_openat] = localOpenAt;
    sys_call_table[__NR_read] = localRead;
    sys_call_table[__NR_write] = localWrite;
    sys_call_table[__NR_close] = localClose;
    setRO();

    printk("[info] %s. overwrite complete, module is ready. \n", __func__);

    return 0;
}

static void __exit exit(void)
{
    setRW();
    printk("[info] %s. restore functions pointers with old functions \n", __func__);
    sys_call_table[__NR_openat] = old_openat;
    sys_call_table[__NR_read] = old_read;
    sys_call_table[__NR_write] = old_write;
    sys_call_table[__NR_close] = old_close;
    setRO();

    printk("[info] %s. restore complete, module is exit. \n", __func__);
}

module_init(init);
module_exit(exit);