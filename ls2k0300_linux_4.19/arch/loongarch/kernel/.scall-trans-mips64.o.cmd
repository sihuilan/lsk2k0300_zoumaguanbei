cmd_arch/loongarch/kernel/scall-trans-mips64.o := loongarch64-linux-gnu-gcc -Wp,-MD,arch/loongarch/kernel/.scall-trans-mips64.o.d -nostdinc -isystem /opt/ls_2k0300_env/loongson-gnu-toolchain-8.3-x86_64-loongarch64-linux-gnu-rc1.6/bin/../lib/gcc/loongarch64-linux-gnu/8.3.0/include -I./arch/loongarch/include -I./arch/loongarch/include/generated  -I./include -I./arch/loongarch/include/uapi -I./arch/loongarch/include/generated/uapi -I./include/uapi -I./include/generated/uapi -include ./include/linux/kconfig.h -D__KERNEL__ -DCC_USING_PATCHABLE_FUNCTION_ENTRY -DVMLINUX_LOAD_ADDRESS=0x9000000000200000 -DDATAOFFSET=0 -D__ASSEMBLY__ -fno-PIE -mno-check-zero-division -mabi=lp64 -G0 -pipe -msoft-float -DGAS_HAS_SET_HARDFLOAT -Wa,-msoft-float -ffreestanding -ULOONGARCHEB -U_LOONGARCHEB -U__LOONGARCHEB -U__LOONGARCHEB__ -ULOONGARCHEL -U_LOONGARCHEL -U__LOONGARCHEL -U__LOONGARCHEL__ -DLOONGARCHEL -D_LOONGARCHEL -D__LOONGARCHEL -D__LOONGARCHEL__ -U_LOONGARCH_ISA -D_LOONGARCH_ISA=_LOONGARCH_ISA_LOONGARCH64 -I./arch/loongarch/include/asm/mach-la64 -I./arch/loongarch/include/asm/mach-generic -Wa,-mla-global-with-pcrel  -c -o arch/loongarch/kernel/.tmp_scall-trans-mips64.o arch/loongarch/kernel/scall-trans-mips64.S

source_arch/loongarch/kernel/scall-trans-mips64.o := arch/loongarch/kernel/scall-trans-mips64.S

deps_arch/loongarch/kernel/scall-trans-mips64.o := \
  include/linux/kconfig.h \
    $(wildcard include/config/cpu/big/endian.h) \
    $(wildcard include/config/booger.h) \
    $(wildcard include/config/foo.h) \
  include/linux/errno.h \
  include/uapi/linux/errno.h \
  arch/loongarch/include/asm/errno.h \
  arch/loongarch/include/generated/uapi/asm/errno.h \
  include/uapi/asm-generic/errno.h \
  include/uapi/asm-generic/errno-base.h \
  arch/loongarch/include/asm/asm.h \
    $(wildcard include/config/cpu/has/prefetch.h) \
    $(wildcard include/config/sgi/ip28.h) \
  arch/loongarch/include/uapi/asm/abidefs.h \
  arch/loongarch/include/asm/asmmacro.h \
    $(wildcard include/config/32bit.h) \
    $(wildcard include/config/64bit.h) \
    $(wildcard include/config/cpu/has/lbt.h) \
  arch/loongarch/include/asm/asm-offsets.h \
  include/generated/asm-offsets.h \
  arch/loongarch/include/asm/asmmacro-64.h \
  arch/loongarch/include/asm/regdef.h \
  arch/loongarch/include/asm/fpregdef.h \
  arch/loongarch/include/asm/loongarchregs.h \
    $(wildcard include/config/page/size/4kb.h) \
    $(wildcard include/config/page/size/16kb.h) \
    $(wildcard include/config/page/size/64kb.h) \
  include/linux/bits.h \
  arch/loongarch/include/uapi/asm/bitsperlong.h \
  include/asm-generic/bitsperlong.h \
  include/uapi/asm-generic/bitsperlong.h \
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
  include/linux/types.h \
    $(wildcard include/config/have/uid16.h) \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/arch/dma/addr/t/64bit.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
  include/uapi/linux/types.h \
  arch/loongarch/include/asm/types.h \
  include/asm-generic/int-ll64.h \
  include/uapi/asm-generic/int-ll64.h \
  arch/loongarch/include/generated/uapi/asm/types.h \
  include/uapi/asm-generic/types.h \
  arch/loongarch/include/asm/irqflags.h \
    $(wildcard include/config/trace/irqflags.h) \
  arch/loongarch/include/asm/stackframe.h \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/kgdb.h) \
  include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
    $(wildcard include/config/base/small.h) \
  arch/loongarch/include/asm/addrspace.h \
  include/linux/const.h \
  include/uapi/linux/const.h \
  arch/loongarch/include/asm/thread_info.h \
  arch/loongarch/include/asm/unwind_hints.h \
    $(wildcard include/config/stack/validation.h) \
  arch/loongarch/include/asm/orc_types.h \
  include/linux/compiler.h \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/debug/entry.h) \
    $(wildcard include/config/kasan.h) \
  arch/loongarch/include/asm/unistd.h \
  arch/loongarch/include/uapi/asm/unistd.h \
  include/asm-generic/unistd.h \
  include/uapi/asm-generic/unistd.h \
    $(wildcard include/config/mmu.h) \

arch/loongarch/kernel/scall-trans-mips64.o: $(deps_arch/loongarch/kernel/scall-trans-mips64.o)

$(deps_arch/loongarch/kernel/scall-trans-mips64.o):
