cmd_arch/loongarch/boot/dts/loongson/seekfree_smart_car_pai_99.dtb := mkdir -p arch/loongarch/boot/dts/loongson/ ; loongarch64-linux-gnu-gcc -E -Wp,-MD,arch/loongarch/boot/dts/loongson/.seekfree_smart_car_pai_99.dtb.d.pre.tmp -nostdinc -I./scripts/dtc/include-prefixes -undef -D__DTS__ -x assembler-with-cpp -o arch/loongarch/boot/dts/loongson/.seekfree_smart_car_pai_99.dtb.dts.tmp arch/loongarch/boot/dts/loongson/seekfree_smart_car_pai_99.dts ; ./scripts/dtc/dtc -O dtb -o arch/loongarch/boot/dts/loongson/seekfree_smart_car_pai_99.dtb -b 0 -iarch/loongarch/boot/dts/loongson/ -i./scripts/dtc/include-prefixes -Wno-unit_address_vs_reg -Wno-unit_address_format -Wno-avoid_unnecessary_addr_size -Wno-alias_paths -Wno-graph_child_address -Wno-graph_port -Wno-simple_bus_reg -Wno-unique_unit_address -Wno-pci_device_reg  -d arch/loongarch/boot/dts/loongson/.seekfree_smart_car_pai_99.dtb.d.dtc.tmp arch/loongarch/boot/dts/loongson/.seekfree_smart_car_pai_99.dtb.dts.tmp ; cat arch/loongarch/boot/dts/loongson/.seekfree_smart_car_pai_99.dtb.d.pre.tmp arch/loongarch/boot/dts/loongson/.seekfree_smart_car_pai_99.dtb.d.dtc.tmp > arch/loongarch/boot/dts/loongson/.seekfree_smart_car_pai_99.dtb.d

source_arch/loongarch/boot/dts/loongson/seekfree_smart_car_pai_99.dtb := arch/loongarch/boot/dts/loongson/seekfree_smart_car_pai_99.dts

deps_arch/loongarch/boot/dts/loongson/seekfree_smart_car_pai_99.dtb := \
  arch/loongarch/boot/dts/loongson/loongson_2k0300.dtsi \
  scripts/dtc/include-prefixes/dt-bindings/interrupt-controller/irq.h \
  scripts/dtc/include-prefixes/dt-bindings/gpio/gpio.h \
  scripts/dtc/include-prefixes/dt-bindings/pinctrl/ls2k300-pinfunc.h \
  arch/loongarch/boot/dts/loongson/2k0300-pinctrl.dtsi \

arch/loongarch/boot/dts/loongson/seekfree_smart_car_pai_99.dtb: $(deps_arch/loongarch/boot/dts/loongson/seekfree_smart_car_pai_99.dtb)

$(deps_arch/loongarch/boot/dts/loongson/seekfree_smart_car_pai_99.dtb):
