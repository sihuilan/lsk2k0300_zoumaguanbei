cmd_arch/loongarch/vdso/vdso.lds := loongarch64-linux-gnu-gcc -E -Wp,-MD,arch/loongarch/vdso/.vdso.lds.d -nostdinc -isystem /opt/ls_2k0300_env/loongson-gnu-toolchain-8.3-x86_64-loongarch64-linux-gnu-rc1.6/bin/../lib/gcc/loongarch64-linux-gnu/8.3.0/include -I./arch/loongarch/include -I./arch/loongarch/include/generated  -I./include -I./arch/loongarch/include/uapi -I./arch/loongarch/include/generated/uapi -I./include/uapi -I./include/generated/uapi -include ./include/linux/kconfig.h -I./arch/loongarch/include/asm/mach-la64 -I./arch/loongarch/include/asm/mach-generic   -msoft-float -D__VDSO__ -mabi=lp64   -P -Uloongarch -D__ASSEMBLY__ -DLINKER_SCRIPT -o arch/loongarch/vdso/vdso.lds arch/loongarch/vdso/vdso.lds.S

source_arch/loongarch/vdso/vdso.lds := arch/loongarch/vdso/vdso.lds.S

deps_arch/loongarch/vdso/vdso.lds := \
  include/linux/kconfig.h \
    $(wildcard include/config/cpu/big/endian.h) \
    $(wildcard include/config/booger.h) \
    $(wildcard include/config/foo.h) \

arch/loongarch/vdso/vdso.lds: $(deps_arch/loongarch/vdso/vdso.lds)

$(deps_arch/loongarch/vdso/vdso.lds):
