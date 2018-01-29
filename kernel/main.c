#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/fs.h>
#include <linux/interrupt.h>
#include "main.h"

MODULE_LICENSE("GPL");


/* You may refer to Intel Sofware Manual for detailed description. */
struct idt_entry
{
	unsigned short lower16; /*lower 16 bits of interrupt handler */
	unsigned short sel;
	unsigned short flags:13;
	unsigned short dpl:2;
	unsigned short present:1;
	unsigned short higher16; /*higher 16 bits of interrupt handler */
} __attribute__((packed));

struct idt_desc
{
	unsigned short size;   /* size of idt table */
	unsigned long address; /* base address of idt table */
} __attribute__((packed));

struct ex_handler
{
	unsigned short sel;
	unsigned long pc;
} __attribute__((packed));


struct ex_handler orig_handler = {0, 0};

/* write the base and size of idt table */
static void imp_load_idt(const struct idt_desc *dtr)
{
	asm volatile("lidt %0"::"m" (*dtr));
}

/* read the base and size of idt table  */
static void imp_store_idt(struct idt_desc *dtr)
{
	asm volatile("sidt %0":"=m" (*dtr));
}

static void unregister_handler(void)
{
	/* stop tracking divbyzero if enabled */
	if (orig_handler.pc != 0)
	{
	}
}

static void register_handler(struct ex_handler *h)
{
	/* start tracking divbyzero if not enabled */
	if (orig_handler.pc == 0)
	{
	}
}

static long device_ioctl(struct file *file, unsigned int ioctl, unsigned long arg)
{
	void __user *argp = (void __user*)arg;
	struct ex_handler h = {0, 0};

	switch (ioctl)
	{
		case REGISTER_HANDLER:
			/* TODO: copy argument from user */
			register_handler(&h);
			break;
		case UNREGISTER_HANDLER:
			unregister_handler();
			break;
		default:
			printk("NOT a valid ioctl\n");
			return 1;
	}
	return 0;
}

static struct file_operations fops = 
{
	.unlocked_ioctl = device_ioctl,
};

int start_module(void)
{
	if (register_chrdev(MAJOR_NUM, MODULE_NAME, &fops) < 0)
	{
		printk("PANIC: aos module loading failed\n");
		return 1;
	}
	printk("aos module loaded successfully\n");
	return 0;
}

void exit_module(void)
{
	unregister_handler();
	unregister_chrdev(MAJOR_NUM, MODULE_NAME);
	printk("aos module unloaded successfully\n");
}

module_init(start_module);
module_exit(exit_module);
