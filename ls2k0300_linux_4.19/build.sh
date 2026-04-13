#!/bin/bash 
echo "执行自动编译+scp传输"

export PATH=/opt/ls_2k0300_env/loongson-gnu-toolchain-8.3-x86_64-loongarch64-linux-gnu-rc1.6/bin:$PATH
# 内核编译不出时可以取消注释试试
export ARCH=loongarch
export CROSS_COMPILE=loongarch64-linux-gnu-

make -j12

scp -O -r vmlinuz root@172.20.10.2:/boot

sync
