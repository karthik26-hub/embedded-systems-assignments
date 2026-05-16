# Makefile - RPi5 Track 1, E3: Boot Sequence Logger
#
# Usage:
#   make              - build boot_logger.ko
#   make load         - basic load
#   make verbose      - load with verbose=1 (prints cmdline + UTS info)
#   make test         - read all sysfs attributes
#   make watch        - poll uptime_ms every second
#   make unload       - rmmod
#   make clean        - remove build artifacts

obj-m += boot_logger.o

KDIR  := /lib/modules/$(shell uname -r)/build
SYSFS := /sys/kernel/boot_logger

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

load:
	sudo insmod boot_logger.ko
	@echo "--- dmesg tail ---"
	@dmesg | tail -12

verbose:
	sudo insmod boot_logger.ko verbose=1
	@echo "--- dmesg tail ---"
	@dmesg | tail -16

unload:
	sudo rmmod boot_logger
	@dmesg | tail -3

test:
	@echo "=== /sys/kernel/boot_logger/info ==="
	@cat $(SYSFS)/info
	@echo ""
	@echo "=== /sys/kernel/boot_logger/uptime_ms ==="
	@cat $(SYSFS)/uptime_ms
	@echo ""
	@echo "=== /sys/kernel/boot_logger/cmdline ==="
	@cat $(SYSFS)/cmdline

watch:
	@echo "Polling uptime_ms every second (Ctrl+C to stop):"
	@while true; do \
		printf "uptime: $$(cat $(SYSFS)/uptime_ms) ms\r"; \
		sleep 1; \
	done

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
