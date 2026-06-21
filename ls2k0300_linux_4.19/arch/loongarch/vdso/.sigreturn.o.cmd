cmd_arch/loongarch/vdso/sigreturn.o := loongarch64-linux-gnu-gcc -Wp,-MD,arch/loongarch/vdso/.sigreturn.o.d -nostdinc -isystem /opt/ls_2k0300_env/loongson-gnu-toolchain-8.3-x86_64-loongarch64-linux-gnu-rc1.6/bin/../lib/gcc/loongarch64-linux-gnu/8.3.0/include -I./arch/loongarch/include -I./arch/loongarch/include/generated  -I./include -I./arch/loongarch/include/uapi -I./arch/loongarch/include/generated/uapi -I./include/uapi -I./include/generated/uapi -include ./include/linux/kconfig.h -D__KERNEL__ -DCC_USING_PATCHABLE_FUNCTION_ENTRY -DVMLINUX_LOAD_ADDRESS=0x9000000000200000 -DDATAOFFSET=0 -I./arch/loongarch/include/asm/mach-la64 -I./arch/loongarch/include/asm/mach-generic -msoft-float -D__VDSO__ -D__ASSEMBLY__ -Wa,-gdwarf-2 -mabi=lp64 -Wa,-mla-global-with-pcrel  -c -o arch/loongarch/vdso/.tmp_sigreturn.o arch/loongarch/vdso/sigreturn.S

source_arch/loongarch/vdso/sigreturn.o := arch/loongarch/vdso/sigreturn.S

deps_arch/loongarch/vdso/sigreturn.o := \
  include/linux/kconfig.h \
    $(wildcard include/config/cpu/big/endian.h) \
    $(wildcard include/config/booger.h) \
    $(wildcard include/config/foo.h) \
  arch/loongarch/vdso/vdso.h \
    $(wildcard include/config/64bit.h) \
    $(wildcard include/config/32bit.h) \
  arch/loongarch/include/uapi/asm/abidefs.h \
  include/linux/linkage.h \
  include/linux/compiler_types.h \
    $(wildcard include/config/have/arch/compiler/h.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
  include/linux/stringify.h \
  include/linux/export.h \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/modversions.h) \
    $(wildcard include/config/module/rel/crcs.h) \
    $(wildcard include/config/have/arch/prel32/relocations.h) \
    $(wildcard include/config/trim/unused/ksyms.h) \
    $(wildcard include/config/unused/symbols.h) \
  arch/loongarch/include/asm/linkage.h \
  arch/loongarch/include/uapi/asm/unistd.h \
  include/asm-generic/unistd.h \
  include/uapi/asm-generic/unistd.h \
    $(wildcard include/config/mmu.h) \
  arch/loongarch/include/uapi/asm/bitsperlong.h \
  include/asm-generic/bitsperlong.h \
  include/uapi/asm-generic/bitsperlong.h \
  arch/loongarch/include/asm/regdef.h \
  arch/loongarch/include/asm/asm.h \
    $(wildcard include/config/cpu/has/prefetch.h) \
    $(wildcard include/config/sgi/ip28.h) \

arch/loongarch/vdso/sigreturn.o: $(deps_arch/loongarch/vdso/sigreturn.o)

$(deps_arch/loongarch/vdso/sigreturn.o):
