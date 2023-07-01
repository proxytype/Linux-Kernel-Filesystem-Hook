#ifndef __HEADERS__
#define __HEADERS__
#include <linux/module.h>
#include <linux/kallsyms.h>
#include <asm/uaccess.h>
#include <linux/moduleparam.h>
#endif

#define READ_ONLY_MASK 0x00010000
#define SYS_CALL_TABLE "sys_call_table"

static void **sys_call_table;
static char path_lookup[256] = "/";

module_param_string(path, path_lookup, sizeof(path_lookup), 0644);
MODULE_PARM_DESC(path, "Description of path_lookup");

MODULE_LICENSE("GPL");

asmlinkage long (*old_read)(const struct pt_regs *);
asmlinkage int (*old_openat)(const struct pt_regs *);
asmlinkage long (*old_write)(const struct pt_regs *);
asmlinkage long (*old_close)(const struct pt_regs *);

static asmlinkage long localClose(const struct pt_regs *regs);
static asmlinkage long localWrite(const struct pt_regs *regs);
static asmlinkage long localRead(const struct pt_regs *regs);
static asmlinkage long localOpenAt(const struct pt_regs *regs);

void setRW(void);
void setRO(void);

static int __init init(void);
static void __exit exit(void);