#kernel path
KDIR :=/home/gaopan/hi3520/kernel/linux-3.0.y

#current path
PWD  :=$(shell pwd)

#compile prefix
CROSS_COMPILE =arm-hisiv100nptl-linux-
CC            =$(CROSS_COMPILE)gcc
LD            =$(CROSS_COMPILE)ld
obj-m :=beep.o
default:
	make ARCH=arm -C $(KDIR) M=$(PWD) modules 
clean:
	rm -rf *.o *.cmd  *.mod.c *.order *.symvers .hello.* .tmp* .bee* *.ko
