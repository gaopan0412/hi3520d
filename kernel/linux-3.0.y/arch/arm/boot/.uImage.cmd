cmd_arch/arm/boot/uImage := /bin/sh /home/gaopan/hi3520/kernel/linux-3.0.y/scripts/mkuboot.sh -A arm -O linux -T kernel -C none -a 0x80008000 -e 0x80008000 -n 'Linux-3.0.8' -d arch/arm/boot/zImage arch/arm/boot/uImage
