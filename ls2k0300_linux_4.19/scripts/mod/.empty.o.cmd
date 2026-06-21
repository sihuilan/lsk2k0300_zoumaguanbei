cmd_scripts/mod/empty.o := loongarch64-linux-gnu-gcc -Wp,-MD,scripts/mod/.empty.o.d -nostdinc -isystem /opt/ls_2k0300_env/loongson-gnu-toolchain-8.3-x86_64-loongarch64-linux-gnu-rc1.6/bin/../lib/gcc/loongarch64-linux-gnu/8.3.0/include -I./arch/loongarch/include -I./arch/loongarch/include/generated  -I./include -I./arch/loongarch/include/uapi -I./arch/loongarch/include/generated/uapi -I./include/uapi -I./include/generated/uapi -include ./include/linux/kconfig.h -include ./include/linux/compiler_types.h -D__KERNEL__ -DCC_USING_PATCHABLE_FUNCTION_ENTRY -DVMLINUX_LOAD_ADDRESS=0x9000000000200000 -DDATAOFFSET=0 -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -fshort-wchar -Werror-implicit-function-declaration -Werror=return-type -Wno-format-security -std=gnu89 -fno-PIE -mno-check-zero-division -mabi=lp64 -G0 -pipe -msoft-float -DGAS_HAS_SET_HARDFLOAT -Wa,-msoft-float -ffreestanding -ULOONGARCHEB -U_LOONGARCHEB -U__LOONGARCHEB -U__LOONGARCHEB__ -ULOONGARCHEL -U_LOONGARCHEL -U__LOONGARCHEL -U__LOONGARCHEL__ -DLOONGARCHEL -D_LOONGARCHEL -D__LOONGARCHEL -D__LOONGARCHEL__ -U_LOONGARCH_ISA -D_LOONGARCH_ISA=_LOONGARCH_ISA_LOONGARCH64 -I./arch/loongarch/include/asm/mach-la64 -I./arch/loongarch/include/asm/mach-generic -fno-asynchronous-unwind-tables -mstrict-align -fno-delete-null-pointer-checks -Wno-frame-address -Wno-format-truncation -Wno-format-overflow -Wno-int-in-bool-context -O2 --param=allow-store-data-races=0 -Wframe-larger-than=1024 -fno-stack-protector -Wno-unused-but-set-variable -Wno-unused-const-variable -fno-var-tracking-assignments -fpatchable-function-entry=2 -Wdeclaration-after-statement -Wno-pointer-sign -Wno-stringop-truncation -Wno-array-bounds -Wno-stringop-overflow -Wno-restrict -Wno-maybe-uninitialized -fno-strict-overflow -fno-merge-all-constants -fmerge-constants -fno-stack-check -fconserve-stack -Werror=implicit-int -Werror=strict-prototypes -Werror=date-time -Werror=incompatible-pointer-types -Werror=designated-init -fmacro-prefix-map=./= -Wno-packed-not-aligned  -Wa,-mla-global-with-pcrel  -DKBUILD_BASENAME='"empty"' -DKBUILD_MODNAME='"empty"' -c -o scripts/mod/.tmp_empty.o scripts/mod/empty.c

source_scripts/mod/empty.o := scripts/mod/empty.c

deps_scripts/mod/empty.o := \
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

scripts/mod/empty.o: $(deps_scripts/mod/empty.o)

$(deps_scripts/mod/empty.o):
