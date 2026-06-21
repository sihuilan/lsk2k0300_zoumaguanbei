cmd_drivers/gpu/drm/mwv206/mwv206hal/mwv206devconfigreader.o := loongarch64-linux-gnu-gcc -Wno-missing-field-initializers -O2 -Wl,-z,relro -z,now -Wl,-z,-Wstramplines,noexecstack -fPIC -D_FORTIFY_SOURCE=2 -fno-strict-aliasing -Wall -W -Wextra -Wno-unused-label -Wno-unused-function -Wno-pointer-sign -Wno-unused-parameter -Wno-missing-braces -Wno-deprecated-declarations -Werror -Wno-sign-compare -Wp,-MD,drivers/gpu/drm/mwv206/mwv206hal/.mwv206devconfigreader.o.d -nostdinc -isystem /opt/ls_2k0300_env/loongson-gnu-toolchain-8.3-x86_64-loongarch64-linux-gnu-rc1.6/bin/../lib/gcc/loongarch64-linux-gnu/8.3.0/include -I./arch/loongarch/include -I./arch/loongarch/include/generated  -I./include -I./arch/loongarch/include/uapi -I./arch/loongarch/include/generated/uapi -I./include/uapi -I./include/generated/uapi -include ./include/linux/kconfig.h -include ./include/linux/compiler_types.h -D__KERNEL__ -DCC_USING_PATCHABLE_FUNCTION_ENTRY -DVMLINUX_LOAD_ADDRESS=0x9000000000200000 -DDATAOFFSET=0 -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -fshort-wchar -Werror-implicit-function-declaration -Werror=return-type -Wno-format-security -std=gnu89 -fno-PIE -mno-check-zero-division -mabi=lp64 -G0 -pipe -msoft-float -DGAS_HAS_SET_HARDFLOAT -Wa,-msoft-float -ffreestanding -ULOONGARCHEB -U_LOONGARCHEB -U__LOONGARCHEB -U__LOONGARCHEB__ -ULOONGARCHEL -U_LOONGARCHEL -U__LOONGARCHEL -U__LOONGARCHEL__ -DLOONGARCHEL -D_LOONGARCHEL -D__LOONGARCHEL -D__LOONGARCHEL__ -U_LOONGARCH_ISA -D_LOONGARCH_ISA=_LOONGARCH_ISA_LOONGARCH64 -I./arch/loongarch/include/asm/mach-la64 -I./arch/loongarch/include/asm/mach-generic -fno-asynchronous-unwind-tables -mstrict-align -fno-delete-null-pointer-checks -Wno-frame-address -Wno-format-truncation -Wno-format-overflow -Wno-int-in-bool-context -O2 --param=allow-store-data-races=0 -Wframe-larger-than=1024 -fno-stack-protector -Wno-unused-but-set-variable -Wno-unused-const-variable -fno-var-tracking-assignments -fpatchable-function-entry=2 -Wdeclaration-after-statement -Wno-pointer-sign -Wno-stringop-truncation -Wno-array-bounds -Wno-stringop-overflow -Wno-restrict -Wno-maybe-uninitialized -fno-strict-overflow -fno-merge-all-constants -fmerge-constants -fno-stack-check -fconserve-stack -Werror=implicit-int -Werror=strict-prototypes -Werror=date-time -Werror=incompatible-pointer-types -Werror=designated-init -fmacro-prefix-map=./= -Wno-packed-not-aligned -Wno-sizeof-pointer-memaccess -D__MWV206_SYSLIBPATH__="\"\"" -I/home/seekfree/Desktop/walk-horse-see-tablet/ls2k0300_linux_4.19/drivers/gpu/drm/mwv206 -I/home/seekfree/Desktop/walk-horse-see-tablet/ls2k0300_linux_4.19/drivers/gpu/drm/mwv206/include -I/home/seekfree/Desktop/walk-horse-see-tablet/ls2k0300_linux_4.19/drivers/gpu/drm/mwv206/mwv206linuxdev -I/home/seekfree/Desktop/walk-horse-see-tablet/ls2k0300_linux_4.19/drivers/gpu/drm/mwv206/mwv206hal -DSUPPORT_SND_PCM  -DMODULE -fplt -Wa,-mla-global-with-abs,-mla-local-with-abs  -DKBUILD_BASENAME='"mwv206devconfigreader"' -DKBUILD_MODNAME='"mwv206"' -c -o drivers/gpu/drm/mwv206/mwv206hal/.tmp_mwv206devconfigreader.o drivers/gpu/drm/mwv206/mwv206hal/mwv206devconfigreader.c

source_drivers/gpu/drm/mwv206/mwv206hal/mwv206devconfigreader.o := drivers/gpu/drm/mwv206/mwv206hal/mwv206devconfigreader.c

deps_drivers/gpu/drm/mwv206/mwv206hal/mwv206devconfigreader.o := \
  include/linux/kconfig.h \
    $(wildcard include/config/cpu/big/endian.h) \
    $(wildcard include/config/booger.h) \
    $(wildcard include/config/foo.h) \
  include/linux/compiler_types.h \
    $(wildcard include/config/have/arch/compiler/h.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
  include/linux/compiler-gcc.h \
    $(wildcard include/config/arm64.h) \
    $(wildcard include/config/retpoline.h) \
    $(wildcard include/config/arch/use/builtin/bswap.h) \
  arch/loongarch/include/uapi/asm/byteorder.h \
  include/linux/byteorder/little_endian.h \
  include/uapi/linux/byteorder/little_endian.h \
  include/linux/types.h \
    $(wildcard include/config/have/uid16.h) \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/arch/dma/addr/t/64bit.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  include/uapi/linux/types.h \
  arch/loongarch/include/asm/types.h \
  include/asm-generic/int-ll64.h \
  include/uapi/asm-generic/int-ll64.h \
  arch/loongarch/include/uapi/asm/bitsperlong.h \
  include/asm-generic/bitsperlong.h \
  include/uapi/asm-generic/bitsperlong.h \
  arch/loongarch/include/generated/uapi/asm/types.h \
  include/uapi/asm-generic/types.h \
  include/uapi/linux/posix_types.h \
  include/linux/stddef.h \
  include/uapi/linux/stddef.h \
  include/linux/compiler_types.h \
  arch/loongarch/include/generated/asm/posix_types.h \
  include/uapi/asm-generic/posix_types.h \
  include/linux/swab.h \
  include/uapi/linux/swab.h \
  include/linux/compiler.h \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/stack/validation.h) \
    $(wildcard include/config/debug/entry.h) \
    $(wildcard include/config/kasan.h) \
  arch/loongarch/include/asm/barrier.h \
    $(wildcard include/config/weak/ordering.h) \
    $(wildcard include/config/weak/reordering/beyond/llsc.h) \
    $(wildcard include/config/smp.h) \
  arch/loongarch/include/asm/addrspace.h \
    $(wildcard include/config/32bit.h) \
  include/linux/const.h \
  include/uapi/linux/const.h \
  arch/loongarch/include/asm/loongarchregs.h \
    $(wildcard include/config/page/size/4kb.h) \
    $(wildcard include/config/page/size/16kb.h) \
    $(wildcard include/config/page/size/64kb.h) \
  include/linux/bits.h \
  include/linux/linkage.h \
  include/linux/stringify.h \
  include/linux/export.h \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/modversions.h) \
    $(wildcard include/config/module/rel/crcs.h) \
    $(wildcard include/config/have/arch/prel32/relocations.h) \
    $(wildcard include/config/trim/unused/ksyms.h) \
    $(wildcard include/config/unused/symbols.h) \
  arch/loongarch/include/asm/linkage.h \
  /opt/ls_2k0300_env/loongson-gnu-toolchain-8.3-x86_64-loongarch64-linux-gnu-rc1.6/lib/gcc/loongarch64-linux-gnu/8.3.0/include/larchintrin.h \
  include/asm-generic/barrier.h \
  include/linux/kasan-checks.h \
  arch/loongarch/include/uapi/asm/swab.h \
  include/linux/byteorder/generic.h \
  drivers/gpu/drm/mwv206/mwv206hal/jmspi.h \
  /home/seekfree/Desktop/walk-horse-see-tablet/ls2k0300_linux_4.19/drivers/gpu/drm/mwv206/include/mwv206kdevconfig.h \
  /home/seekfree/Desktop/walk-horse-see-tablet/ls2k0300_linux_4.19/drivers/gpu/drm/mwv206/include/mwv206.h \
  drivers/gpu/drm/mwv206/mwv206hal/mwv206devconfigbin.h \
  /home/seekfree/Desktop/walk-horse-see-tablet/ls2k0300_linux_4.19/drivers/gpu/drm/mwv206/include/mwv206ioctl.h \
  /home/seekfree/Desktop/walk-horse-see-tablet/ls2k0300_linux_4.19/drivers/gpu/drm/mwv206/include/mwv206kdevconfig.h \
  include/uapi/linux/ioctl.h \
  arch/loongarch/include/generated/asm/ioctl.h \
  include/asm-generic/ioctl.h \
  include/uapi/asm-generic/ioctl.h \

drivers/gpu/drm/mwv206/mwv206hal/mwv206devconfigreader.o: $(deps_drivers/gpu/drm/mwv206/mwv206hal/mwv206devconfigreader.o)

$(deps_drivers/gpu/drm/mwv206/mwv206hal/mwv206devconfigreader.o):
