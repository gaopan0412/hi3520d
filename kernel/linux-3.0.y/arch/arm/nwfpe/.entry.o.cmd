cmd_arch/arm/nwfpe/entry.o := arm-hisiv100nptl-linux-gcc -Wp,-MD,arch/arm/nwfpe/.entry.o.d  -nostdinc -isystem /opt/hisi-linux-nptl/arm-hisiv100-linux/bin/../lib/gcc/arm-hisiv100-linux-uclibcgnueabi/4.4.1/include -I/home/gaopan/hi3520/kernel/linux-3.0.y/arch/arm/include -Iarch/arm/include/generated -Iinclude  -include include/generated/autoconf.h -D__KERNEL__ -mlittle-endian -Iarch/arm/mach-hi3520d/include -D__ASSEMBLY__ -mabi=aapcs-linux -mno-thumb-interwork  -D__LINUX_ARM_ARCH__=7 -march=armv7-a  -include asm/unified.h -msoft-float        -c -o arch/arm/nwfpe/entry.o arch/arm/nwfpe/entry.S

source_arch/arm/nwfpe/entry.o := arch/arm/nwfpe/entry.S

deps_arch/arm/nwfpe/entry.o := \
  /home/gaopan/hi3520/kernel/linux-3.0.y/arch/arm/include/asm/unified.h \
    $(wildcard include/config/arm/asm/unified.h) \
    $(wildcard include/config/thumb2/kernel.h) \
  /home/gaopan/hi3520/kernel/linux-3.0.y/arch/arm/include/asm/asm-offsets.h \
  include/generated/asm-offsets.h \

arch/arm/nwfpe/entry.o: $(deps_arch/arm/nwfpe/entry.o)

$(deps_arch/arm/nwfpe/entry.o):
