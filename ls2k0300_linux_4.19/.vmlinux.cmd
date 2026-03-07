cmd_vmlinux := /bin/bash scripts/link-vmlinux.sh loongarch64-linux-gnu-ld  -m elf64loongarch  -G0 -static -n -nostdlib  --build-id ;  make -f ./arch/loongarch/Makefile.postlink vmlinux
