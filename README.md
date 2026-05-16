# Embedded Systems Assignments

Working through a structured embedded systems curriculum on real hardware.
No simulators. Everything here ran on actual silicon and either worked or didn't.

## Hardware

| Board | Role |
|-------|------|
| Raspberry Pi 5 (8GB) | Linux BSP, drivers, Bluetooth, networking |
| NXP i.MX8MP PhyBoard Pollux | AOSP/AAOS, Bluetooth HAL, automotive stack |
| RPi Touch Display 2 | UI layer on RPi5 |

## Stack I'm working across

Kernel modules → device drivers → HAL → middleware → application layer.
Classic Bluetooth (HFP, A2DP, AVRCP) and BLE (GATT, LE Audio) on both boards.
SOME/IP, DoIP, CAN for the automotive side.

## What's actually in here

Every folder has:
- The code I wrote
- A `notes.md` with every error I hit and what actually fixed it
- Real dmesg/logcat output, not sanitized

The `ERRORS.md` at root is a running reference of kernel 6.12 + RPi5 specific
issues — things that broke because tutorials were written for older kernels or RPi4.
Saving future-me (and maybe you) some hours.

## Track progress

### Track 1 — Linux BSP & Driver Development (RPi5, kernel 6.12)

| Assignment | What | Status |
|------------|------|--------|
| E1 | Hello World LKM — module params, printk | ✅ |
| E2 | GPIO17 LED — led_classdev, RP1 GPIO API | ✅ |
| E3 | Boot logger — ktime, sysfs kobject | ✅ |
| M1 | Char device — cdev, fops, ioctl, mutex | ✅ |
| M2 | I2C sensor driver | 🔧 |
| M3 | systemd module autoload | ⬜ |
| H1 | Port RPi4 driver → RPi5 | ⬜ |
| H2 | DMA char driver | ⬜ |
| H3 | Platform device + IRQ | ⬜ |

### Track 2 — Automotive Ethernet Gateway | ⬜
### Track 3 — SOME/IP ECU Simulation | ⬜
### Track 4 — DoIP Gateway | ⬜
### Track 5 — Bluetooth Middleware (BlueZ) | ⬜
### Track 6 — Embedded Middleware Framework | ⬜
### Track 7 — Bluetooth Profiles Deep Dive | ⬜

## Background

B.Tech ECE (2023) + PG Diploma in Embedded Systems Design, CDAC ACTS Pune (2025).
Currently working on Bluetooth infotainment integration at Karnex Software Solutions, Pune.
This repo is me going deeper on the kernel and automotive middleware side.

## Kernel version note

Everything on RPi5 runs on **kernel 6.12.75+rpt-rpi-2712**.
A lot of GPIO and LED trigger APIs changed between 5.x and 6.12.
Check `ERRORS.md` before copy-pasting anything from Stack Overflow.
