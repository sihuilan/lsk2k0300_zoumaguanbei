cmd_arch/loongarch/vdso/elf.o := loongarch64-linux-gnu-gcc -Wp,-MD,arch/loongarch/vdso/.elf.o.d -nostdinc -isystem /opt/ls_2k0300_env/loongson-gnu-toolchain-8.3-x86_64-loongarch64-linux-gnu-rc1.6/bin/../lib/gcc/loongarch64-linux-gnu/8.3.0/include -I./arch/loongarch/include -I./arch/loongarch/include/generated  -I./include -I./arch/loongarch/include/uapi -I./arch/loongarch/include/generated/uapi -I./include/uapi -I./include/generated/uapi -include ./include/linux/kconfig.h -D__KERNEL__ -DCC_USING_PATCHABLE_FUNCTION_ENTRY -DVMLINUX_LOAD_ADDRESS=0x9000000000200000 -DDATAOFFSET=0 -I./arch/loongarch/include/asm/mach-la64 -I./arch/loongarch/include/asm/mach-generic -msoft-float -D__VDSO__ -D__ASSEMBLY__ -Wa,-gdwarf-2 -mabi=lp64 -Wa,-mla-global-with-pcrel  -c -o arch/loongarch/vdso/.tmp_elf.o arch/loongarch/vdso/elf.S

source_arch/loongarch/vdso/elf.o := arch/loongarch/vdso/elf.S

deps_arch/loongarch/vdso/elf.o := \
  include/linux/kconfig.h \
    $(wildcard include/config/cpu/big/endian.h) \
    $(wildcard include/config/booger.h) \
    $(wildcard include/config/foo.h) \
  arch/loongarch/vdso/vdso.h \
    $(wildcard include/config/64bit.h) \
    $(wildcard include/config/32bit.h) \
  arch/loongarch/include/uapi/asm/abidefs.h \
  include/linux/elfnote.h \
  include/generated/uapi/linux/version.h \

arch/loongarch/vdso/elf.o: $(deps_arch/loongarch/vdso/elf.o)

$(deps_arch/loongarch/vdso/elf.o):
