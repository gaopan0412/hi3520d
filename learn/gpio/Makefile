#kernel path
KDIR :=/home/gaopan/project/linux_kernel/linux-3.0.y

#current path
PWD  :=$(shell pwd)

#compile prefix
CROSS_COMPILE =/opt/hisi-linux-nptl/arm-hisiv100-linux/target/bin/arm-hisiv100nptl-linux-
CC            =$(CROSS_COMPILE)gcc
LD            =$(CROSS_COMPILE)ld
obj-m :=gpio.o
default:
	make ARCH=arm -C $(KDIR) M=$(PWD) modules 
clean:
	rm -rf *.o *.cmd  *.mod.c *.order *.symvers .hello.* .tmp* .gp* *.ko
