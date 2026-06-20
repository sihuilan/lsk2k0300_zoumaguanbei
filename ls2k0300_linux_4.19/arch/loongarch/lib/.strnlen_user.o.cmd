cmd_arch/loongarch/lib/strnlen_user.o := loongarch64-linux-gnu-gcc -Wp,-MD,arch/loongarch/lib/.strnlen_user.o.d -nostdinc -isystem /opt/ls_2k0300_env/loongson-gnu-toolchain-8.3-x86_64-loongarch64-linux-gnu-rc1.6/bin/../lib/gcc/loongarch64-linux-gnu/8.3.0/include -I./arch/loongarch/include -I./arch/loongarch/include/generated  -I./include -I./arch/loongarch/include/uapi -I./arch/loongarch/include/generated/uapi -I./include/uapi -I./include/generated/uapi -include ./include/linux/kconfig.h -D__KERNEL__ -DCC_USING_PATCHABLE_FUNCTION_ENTRY -DVMLINUX_LOAD_ADDRESS=0x9000000000200000 -DDATAOFFSET=0 -D__ASSEMBLY__ -fno-PIE -mno-check-zero-division -mabi=lp64 -G0 -pipe -msoft-float -DGAS_HAS_SET_HARDFLOAT -Wa,-msoft-float -ffreestanding -ULOONGARCHEB -U_LOONGARCHEB -U__LOONGARCHEB -U__LOONGARCHEB__ -ULOONGARCHEL -U_LOONGARCHEL -U__LOONGARCHEL -U__LOONGARCHEL__ -DLOONGARCHEL -D_LOONGARCHEL -D__LOONGARCHEL -D__LOONGARCHEL__ -U_LOONGARCH_ISA -D_LOONGARCH_ISA=_LOONGARCH_ISA_LOONGARCH64 -I./arch/loongarch/include/asm/mach-la64 -I./arch/loongarch/include/asm/mach-generic -Wa,-mla-global-with-pcrel  -c -o arch/loongarch/lib/.tmp_strnlen_user.o arch/loongarch/lib/strnlen_user.S

source_arch/loongarch/lib/strnlen_user.o := arch/loongarch/lib/strnlen_user.S

deps_arch/loongarch/lib/strnlen_user.o := \
  include/linux/kconfig.h \
    $(wildcard include/config/cpu/big/endian.h) \
    $(wildcard include/config/booger.h) \
    $(wildcard include/config/foo.h) \
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
  arch/loongarch/include/asm/asm.h \
    $(wildcard include/config/cpu/has/prefetch.h) \
    $(wildcard include/config/sgi/ip28.h) \
  arch/loongarch/include/uapi/asm/abidefs.h \
  arch/loongarch/include/generated/asm/export.h \
  include/asm-generic/export.h \
    $(wildcard include/config/64bit.h) \
  arch/loongarch/include/asm/regdef.h \
  arch/loongarch/include/asm/asm-extable.h \

arch/loongarch/lib/strnlen_user.o: $(deps_arch/loongarch/lib/strnlen_user.o)

$(deps_arch/loongarch/lib/strnlen_user.o):
