
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/ioctl.h>
#include <linux/types.h>
#include <linux/timer.h>
#include <linux/miscdevice.h>
#include <linux/watchdog.h>
#include <linux/fs.h>
#include <linux/notifier.h>
#include <linux/reboot.h>
#include <linux/init.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/kthread.h>
#include <linux/string.h>
#include <linux/sched.h>

typedef struct 
{
	unsigned int group;
	unsigned int pin;
	unsigned int direct;
	unsigned int value;
} gpio_params;

// GPIO IOTL
#define GPIO_SET_PINMUX			0x1100
#define GPIO_SET_DATA_OUTPUT    0x1101
#define GPIO_READ_DATA_INPUT    0x1102
#define GPIO_SET_DATA_DIRECT    0x1103

#define REG_OFFSET   			(0x10000)

/*
#define HIREG_SCTL_BASE			0x20050000

#define HIREG_ADC_BASE			0x200b0000
#define HIREG_ADC_CTRL			0x004
#define HIREG_ADC_MODE			0x008
#define HIREG_ADC_RESULT		0x01c

#define HIREG_PINMUX_GPIO		0x200f0000
#define PINMUX_GPIO0_6			0x0138
#define PINMUX_GPIO5_2			0x00bc
#define PINMUX_GPIO5_3			0x00c0
#define PINMUX_GPIO7_5			0x00c4
#define PINMUX_GPIO9_0			0x00c8
#define PINMUX_GPIO9_1			0x00cc
#define PINMUX_GPIO9_2			0x00d0
#define PINMUX_GPIO9_3			0x00d4
#define PINMUX_GPIO9_4			0x00d8
#define PINMUX_GPIO9_5			0x00dc
#define PINMUX_GPIO9_6			0x00e0
#define PINMUX_GPIO9_7			0x00e4
*/

#define REG_GPIO_DIR   			(0x400)
#define HIREG_GPIO0_BASE 		0x20150000
#define HIREG_GPIO0_PIN0		0x0004
#define HIREG_GPIO0_PIN1		0x0008
#define HIREG_GPIO0_PIN2		0x0010
#define HIREG_GPIO0_PIN3		0x0020
#define HIREG_GPIO0_PIN4		0x0040
#define HIREG_GPIO0_PIN5		0x0080
#define HIREG_GPIO0_PIN6		0x0100
#define HIREG_GPIO0_PIN7		0x0200

#define HIREG_GPIO8_BASE 		0x201D0000
#define HIREG_GPIO8_PIN0		0x0004
#define HIREG_GPIO8_PIN1		0x0008
#define HIREG_GPIO8_PIN2		0x0010
#define HIREG_GPIO8_PIN3		0x0020
#define HIREG_GPIO8_PIN4		0x0040
#define HIREG_GPIO8_PIN5		0x0080
#define HIREG_GPIO8_PIN6		0x0100
#define HIREG_GPIO8_PIN7		0x0200

#define GPIO_MODULE_VERSION_STRING "GPIO @Hi3518v100"

/* debug */
#define HIGPIO_PFX "GPIO: "

#define	HI3520D_PINMUXREG_BASEADDR	0x200f0000
#define	HI3520D_VGAHSMUXREG_OFFSET	0x00c
#define	HI3520D_VGAVSMUXREG_OFFSET	0x010


/* define IO */
void __iomem *reg_pinmux_base_va;
void __iomem *reg_gpio0_base_va;
void __iomem *reg_gpio8_base_va;

static int higpio_open(struct inode *inode, struct file *file)
{
    return 0;
}

static int SetPinmux(void)
{
	void *baseaddr = reg_pinmux_base_va;
	
	printk("vga pin mux value : [%lx][%lx]\n", *(unsigned long *)(baseaddr + HI3520D_VGAHSMUXREG_OFFSET), *(unsigned long *)(baseaddr + HI3520D_VGAVSMUXREG_OFFSET));
	printk("uart2 pin mux value : [%lx][%lx]\n", *(unsigned long *)(baseaddr + 0x5c), *(unsigned long *)(baseaddr + 0x60));
	*(unsigned long *)(baseaddr + HI3520D_VGAHSMUXREG_OFFSET) = 0x1;
	*(unsigned long *)(baseaddr + HI3520D_VGAVSMUXREG_OFFSET) = 0x1;
	*(unsigned long *)(baseaddr + 0x5c) = 0x1;
	*(unsigned long *)(baseaddr + 0x60) = 0x1;
	printk("after set, vga pin mux value : [%lx][%lx]\n", *(unsigned long *)(baseaddr + HI3520D_VGAHSMUXREG_OFFSET), *(unsigned long *)(baseaddr + HI3520D_VGAVSMUXREG_OFFSET));
	printk("after set, uart2 pin mux value : [%lx][%lx]\n", *(unsigned long *)(baseaddr + 0x5c), *(unsigned long *)(baseaddr + 0x60));
/*	void *baseaddr = reg_pinmux_base_va;
	
	// 00:GPIO0_6, 01:SVB_PWM, 10:TEMP_DQ
	*(unsigned long *)(baseaddr + PINMUX_GPIO0_6) &= 0xFFFFFFFC;

	// 0:GPIO5_2, 1:PWM_OUT0
	*(unsigned long *)(baseaddr + PINMUX_GPIO5_2) &= 0xFFFFFFFE;

	// 0:GPIO5_3, 1:PWM_OUT1
	*(unsigned long *)(baseaddr + PINMUX_GPIO5_3) &= 0xFFFFFFFE;

	// 0:IR_EN, 1:GPIO7_5
	*(unsigned long *)(baseaddr + PINMUX_GPIO7_5) |= 0x01;

	// 0:Reserved, 1:GPIO9_0-7
	*(unsigned long *)(baseaddr + PINMUX_GPIO9_0) |= 0x01;
	*(unsigned long *)(baseaddr + PINMUX_GPIO9_1) |= 0x01;
	*(unsigned long *)(baseaddr + PINMUX_GPIO9_2) |= 0x01;
	*(unsigned long *)(baseaddr + PINMUX_GPIO9_3) |= 0x01;
	*(unsigned long *)(baseaddr + PINMUX_GPIO9_4) |= 0x01;
	
	*(unsigned long *)(baseaddr + PINMUX_GPIO9_5) |= 0x01;
	*(unsigned long *)(baseaddr + PINMUX_GPIO9_6) |= 0x01;
	*(unsigned long *)(baseaddr + PINMUX_GPIO9_7) |= 0x01;
*/
	return 0;
}

static int SetGpioDirect(int group, int gpio, int direct)
{
	void *baseaddr = NULL;
	unsigned char value = 0;

	if (group == 0) {
		baseaddr = reg_gpio0_base_va;
	} else if (group == 8) {
		baseaddr = reg_gpio8_base_va;
	} else {
		printk(KERN_ERR HIGPIO_PFX "(%s:%u) invalid group[%d].\n", __FUNCTION__, __LINE__, group);
		return -1;
	}	
	value = *(unsigned char *)(baseaddr + REG_GPIO_DIR);
	if (direct == 1) 	// in bitx=0
		value &= ~(0x01 << (gpio));
	if (direct == 0)	// out bitx=1
		value |= (0x01 << (gpio));
	*(unsigned char *)(baseaddr + REG_GPIO_DIR) = value;	
	value = *(unsigned char *)(baseaddr + REG_GPIO_DIR);
	return 0;
}

static int SetGpioOutput(int group, int gpio, int level)
{
	void *baseaddr = NULL;
	unsigned long offset = 0;
	unsigned char value = 0;

	if (group == 0) {
		baseaddr = reg_gpio0_base_va;
	} else if (group == 8) {
		baseaddr = reg_gpio8_base_va;
	} else {
        	printk(KERN_ERR HIGPIO_PFX "(%s:%u) invalid group[%d].\n", __FUNCTION__, __LINE__, group);
		return -1;
	}	
	offset = (0x01 << (gpio + 2));	
	value = *(unsigned char *)(baseaddr + offset);
	if (level == 0)
		value &= ~(0x01 << gpio);
	else
		value |= (0x01 << gpio);
	*(unsigned char *)(baseaddr + offset) = value;	
	return 0;
}

static int GetGpioInput(int group, int gpio)
{
	void *baseaddr = NULL;
	unsigned long offset = 0;
	unsigned char value = 0;
	if (group == 0) {
		baseaddr = reg_gpio0_base_va;
	} else if (group == 8) {
		baseaddr = reg_gpio8_base_va;
	} else {
        	printk(KERN_ERR HIGPIO_PFX "(%s:%u) invalid group[%d].\n", __FUNCTION__, __LINE__, group);
		return -1;
	}
	offset = (0x01 << (gpio + 2));
	value = *(unsigned char *)(baseaddr + offset);
	value = (value >> gpio) & 0x01;
	return (value);
}

//static int hidog_ioctl(struct inode *inode, struct file *file,
//    unsigned int cmd, unsigned long arg)
static long higpio_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	gpio_params gm;
	switch (cmd) 
	{
		case GPIO_SET_PINMUX:
			SetPinmux();
			return 0; break;
			
		case GPIO_SET_DATA_DIRECT:
			if (copy_from_user (&gm, (gpio_params *)arg, sizeof(gpio_params)))
				return -EFAULT;
			SetGpioDirect(gm.group, gm.pin, gm.direct);
			return 0; break;

		case GPIO_SET_DATA_OUTPUT:
			if (copy_from_user (&gm, (gpio_params *)arg, sizeof(gpio_params)))
				return -EFAULT;
			SetGpioOutput(gm.group, gm.pin, gm.value);
			return 0; break;			
			
		case GPIO_READ_DATA_INPUT:
			if (copy_from_user (&gm, (gpio_params *)arg, sizeof(gpio_params)))
				return -EFAULT;
			gm.value = GetGpioInput(gm.group, gm.pin);			
			copy_to_user((void *)arg, &gm, sizeof(gpio_params));
			return 0; break;		
			
		default:
			printk(KERN_ERR HIGPIO_PFX "invalid ioctl function[%d].\n", cmd);
			return -EINVAL;
	}
}


static struct file_operations gpio_fops=
{
	.owner = THIS_MODULE,
//	.ioctl = higpio_ioctl,
    .unlocked_ioctl = higpio_ioctl,
	.open = higpio_open,
};
static struct miscdevice higpio_miscdev=
{
    .minor = MISC_DYNAMIC_MINOR,
    .name = "gpio_test",
    .fops = &gpio_fops,
};

static int __init higpio_init(void)
{
    int ret = 0;

    reg_pinmux_base_va = ioremap(HI3520D_PINMUXREG_BASEADDR, REG_OFFSET);
    if (NULL == reg_pinmux_base_va)
    {
        printk(KERN_ERR HIGPIO_PFX "(%s:%u) failed\n", __FUNCTION__, __LINE__);
        return -1;
    }
	
	reg_gpio0_base_va = ioremap(HIREG_GPIO0_BASE, REG_OFFSET);
    if (NULL == reg_gpio0_base_va)
    {
        printk(KERN_ERR HIGPIO_PFX "(%s:%u) failed\n", __FUNCTION__, __LINE__);
	iounmap(reg_pinmux_base_va);
        return -1;
    }
	reg_gpio8_base_va = ioremap(HIREG_GPIO8_BASE, REG_OFFSET);
    if (NULL == reg_gpio8_base_va)
    {
        printk(KERN_ERR HIGPIO_PFX "(%s:%u) failed\n", __FUNCTION__, __LINE__);
	    iounmap(reg_gpio0_base_va);
	    iounmap(reg_pinmux_base_va);
        return -1;
    }
	//SetPinmux();
	
    ret = misc_register(&higpio_miscdev);
    if (ret) {
        printk (KERN_ERR HIGPIO_PFX "cannot register miscdev (err=%d)\n", ret);
        goto watchdog_init_err;
    }

    //printk(KERN_INFO GPIO_MODULE_VERSION_STRING "\n");
    return ret;

watchdog_init_err:
    iounmap(reg_gpio8_base_va);
    iounmap(reg_gpio0_base_va);
    iounmap(reg_pinmux_base_va);
    return ret;
}

static void __exit higpio_exit(void)
{    
    misc_deregister(&higpio_miscdev);
  
    iounmap(reg_gpio8_base_va);
    iounmap(reg_gpio0_base_va);
    iounmap(reg_pinmux_base_va);
    printk("bye bye\n");	
}

module_init(higpio_init);
module_exit(higpio_exit);

MODULE_AUTHOR("XML");
MODULE_DESCRIPTION("GPIO Device Driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS_MISCDEV(WATCHDOG_MINOR);
MODULE_VERSION("HI_VERSION=" GPIO_MODULE_VERSION_STRING);

