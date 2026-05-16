// SPDX-License-Identifier: GPL-2.0
/*
 * boot_logger.c - RPi5 Track 1, E3: Boot Sequence Logger
 *
 * Demonstrates:
 *   - ktime_get_boottime() for boot-relative timestamps
 *   - kobject + sysfs_create_group for custom /sys/kernel/ entries
 *   - UTS namespace queries (kernel version, hostname, arch)
 *   - Boot timeline delta measurement
 *
 * Note: cmdline is not exposed here — read via /proc/cmdline directly.
 *
 * Build:  make clean && make
 * Load:   sudo insmod boot_logger.ko
 *         sudo insmod boot_logger.ko verbose=1
 * Read:
 *   dmesg | grep boot_logger
 *   cat /sys/kernel/boot_logger/info
 *   cat /sys/kernel/boot_logger/uptime_ms
 *   cat /proc/cmdline
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/ktime.h>
#include <linux/timekeeping.h>
#include <linux/utsname.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Karthik Nambiar");
MODULE_DESCRIPTION("RPi5 Track1 E3 - Boot sequence logger with sysfs");
MODULE_VERSION("1.0");

static int verbose = 0;
module_param(verbose, int, 0644);
MODULE_PARM_DESC(verbose, "Verbose output: print UTS info to dmesg (0=off, 1=on)");

static struct kobject *boot_logger_kobj;
static u64 boot_ms;   /* ms since boot at module load */

/* ------------------------------------------------------------------ */
/* sysfs: /sys/kernel/boot_logger/info                                 */
/* ------------------------------------------------------------------ */
static ssize_t info_show(struct kobject *kobj, struct kobj_attribute *attr,
                         char *buf)
{
    struct new_utsname *uts = utsname();
    return sysfs_emit(buf,
        "kernel:   %s\n"
        "hostname: %s\n"
        "arch:     %s\n"
        "boot_ms:  %llu\n",
        uts->release,
        uts->nodename,
        uts->machine,
        boot_ms);
}

/* ------------------------------------------------------------------ */
/* sysfs: /sys/kernel/boot_logger/uptime_ms                            */
/* ------------------------------------------------------------------ */
static ssize_t uptime_ms_show(struct kobject *kobj, struct kobj_attribute *attr,
                               char *buf)
{
    return sysfs_emit(buf, "%llu\n", ktime_to_ms(ktime_get_boottime()));
}

static struct kobj_attribute attr_info   = __ATTR_RO(info);
static struct kobj_attribute attr_uptime = __ATTR_RO(uptime_ms);

static struct attribute *boot_logger_attrs[] = {
    &attr_info.attr,
    &attr_uptime.attr,
    NULL,
};

static struct attribute_group boot_logger_group = {
    .attrs = boot_logger_attrs,
};

/* ------------------------------------------------------------------ */
/* Module init / exit                                                   */
/* ------------------------------------------------------------------ */
static int __init boot_logger_init(void)
{
    int ret;
    u64 now_ms;
    struct new_utsname *uts = utsname();

    boot_ms = ktime_to_ms(ktime_get_boottime());

    printk(KERN_INFO "boot_logger: === loaded at +%llu ms since boot ===\n", boot_ms);
    printk(KERN_INFO "boot_logger: kernel  : %s\n", uts->release);
    printk(KERN_INFO "boot_logger: arch    : %s\n", uts->machine);

    now_ms = ktime_to_ms(ktime_get_boottime());
    printk(KERN_INFO "boot_logger: delta since load: %llu ms\n", now_ms - boot_ms);

    if (verbose) {
        printk(KERN_INFO "boot_logger: hostname: %s\n", uts->nodename);
        printk(KERN_INFO "boot_logger: sysname : %s\n", uts->sysname);
        printk(KERN_INFO "boot_logger: version : %s\n", uts->version);
        printk(KERN_INFO "boot_logger: tip: run 'cat /proc/cmdline' for boot args\n");
    }

    /* Create /sys/kernel/boot_logger/ */
    boot_logger_kobj = kobject_create_and_add("boot_logger", kernel_kobj);
    if (!boot_logger_kobj)
        return -ENOMEM;

    ret = sysfs_create_group(boot_logger_kobj, &boot_logger_group);
    if (ret) {
        kobject_put(boot_logger_kobj);
        return ret;
    }

    printk(KERN_INFO "boot_logger: sysfs ready:\n");
    printk(KERN_INFO "boot_logger:   cat /sys/kernel/boot_logger/info\n");
    printk(KERN_INFO "boot_logger:   cat /sys/kernel/boot_logger/uptime_ms\n");
    return 0;
}

static void __exit boot_logger_exit(void)
{
    u64 unload_ms = ktime_to_ms(ktime_get_boottime());
    sysfs_remove_group(boot_logger_kobj, &boot_logger_group);
    kobject_put(boot_logger_kobj);
    printk(KERN_INFO "boot_logger: unloaded at +%llu ms (resident %llu ms)\n",
           unload_ms, unload_ms - boot_ms);
}

module_init(boot_logger_init);
module_exit(boot_logger_exit);
