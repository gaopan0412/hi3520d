#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <linux/device.h>



#define GPIO8_BASE    0x201D0000
#define MUX_PIN_BASE  0x200f0000

unsigned int DATA_CTL_REG ;
unsigned int MUX_BASE ;
#define OFFSET   	(0x10000)

#define PIN0		0x0004
#define PIN1		0x0008
#define PIN2		0x0010
#define PIN3		0x0020
#define PIN4		0x0040
#define PIN5		0x0080
#define PIN6		0x0100
#define PIN7		0x0200

#if 0
struct beep_param
{
    unsigned int group;
    unsigned int pin;
    unsigned int direct;
    unsigned int value;
};
#endif

struct cdev beep;
dev_t beep_no;
static struct class *beep_class;
static struct device *beep_dev;

static int  beep_open(struct inode *inode, struct file *file)
{
   printk("open beep\n");
   return 0;
 
}
static int set_pin_mux(void)
{

   * ((unsigned int*)(MUX_BASE+0x030)) *= 0x0000;
    //set pin is down that the pin  is gpio
     return 0;
}

static int set_pin_dir(void)
{
  // unsigned int dir = *(unsig)
 * (unsigned int*)( DATA_CTL_REG + 0x400) = 0x01;
   //set directory is output
    return 0;
}

static int set_pin_bit(unsigned int bit)
{
   //set output data is 1
  *(unsigned int *)(DATA_CTL_REG+0X04) = bit;
    return 0;
}

static int ioctl_beep(struct file *file, unsigned int bit)
{
     set_pin_mux();
     set_pin_dir();
     set_pin_bit(bit);
   //ioctl interface
   return 0;
}


static struct file_operations fops=
{
      .owner = THIS_MODULE,
      .open  = beep_open,
      .unlocked_ioctl = ioctl_beep,
};

static int __init beep_init()
{

    DATA_CTL_REG = ioremap(GPIO8_BASE, OFFSET);
    MUX_BASE = ioremap(MUX_PIN_BASE, OFFSET);

    cdev_init(&beep, &fops);
    alloc_chrdev_region(&beep_no, 0, 1, "beep");
    printk("[beep]the beep no:%d\n", beep_no);
    cdev_add(&beep, beep_no, 1);
    beep_class = class_create(THIS_MODULE, "beep_class");
    beep_dev = device_create(beep_class, NULL, beep_no, NULL, "bee_dev");

    printk("[beep] the beep has register\n");
    return 0 ;
}

static __exit beep_exit()
{
    cdev_del(&beep);
    unregister_chrdev_region(beep_no,1);
    device_unregister(beep_dev);
    class_destroy(beep_class);
    printk("[beep] bye bye beep driver\n");
    

}


module_init(beep_init);
module_exit(beep_exit);
MODULE_LICENSE("GPL");

