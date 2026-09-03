# ESP-07S modern Web UI baseline

Baseline date: 2026-09-04

Source commit: `64c1acc`

Firmware version: `2.10h`

## Scope

This baseline freezes the resource and protocol assumptions before the Vue UI is added. Stage 1 does not change firmware behavior or existing HTTP routes.

## Target and toolchain

| Item | Baseline |
| --- | --- |
| Module | Espressif ESP-07S |
| MCU | ESP8266 at 80 MHz |
| Physical flash | 4,194,304 B |
| Declared RAM | 81,920 B |
| Network mode | Wi-Fi |
| OTA | Retained through `ArduinoOTA` |
| Flash layout | `eagle.flash.4m1m.ld` |
| Filesystem region | 1,024,000 B |
| Linked application ceiling | 1,044,464 B |
| PlatformIO platform | `platformio/espressif8266@4.2.1` |
| ESP8266 Arduino Core | 3.1.2 |
| Compiler | xtensa-lx106 GCC 10.3.0 |

The build used PlatformIO's `esp07s` board definition and the repository's current `Config.h`. A temporary local-only wrapper was used because the repository has no committed PlatformIO manifest.

## Current build measurements

| Metric | Value | Interpretation |
| --- | ---: | --- |
| Linked program use | 522,576 B | 50.03% of the 1,044,464 B application ceiling |
| Linked program headroom | 521,888 B | 49.97% before the linker ceiling |
| Generated `firmware.bin` | 526,736 B | Actual OTA/upload artifact size |
| Static RAM use | 43,976 B | 53.68% of declared RAM |
| Static RAM headroom | 37,944 B | Does not equal runtime free heap |
| OTA staging free space estimate | 2,617,344 B | `FS_start` minus current image rounded to a 4 KiB sector |
| Conservative updater maximum | 2,613,248 B | Staging estimate less one 4 KiB safety sector |

The 4M/1M ESP8266 layout does not use two fixed application partitions. OTA stages the new image in the free area between the running image and the filesystem, then the bootloader copies it into place. The linked application ceiling remains the release blocker even when the staging area is larger.

The baseline build completed with one existing warning: `WiFiServer::available()` is deprecated by ESP8266 Arduino Core 3.1.2 in favor of `accept()`.

## Runtime heap baseline

No serial device was connected during Stage 1, so runtime heap values are intentionally not guessed.

Capture the following values on the target after `SmartWebServer ready`, after five minutes idle, during the largest legacy page response, and after the response completes:

| Sample | Free heap | Largest free block | Fragmentation |
| --- | ---: | ---: | ---: |
| Ready | Pending hardware | Pending hardware | Pending hardware |
| Idle for five minutes | Pending hardware | Pending hardware | Pending hardware |
| Peak response transfer | Pending hardware | Pending hardware | Pending hardware |
| Post-transfer recovery | Pending hardware | Pending hardware | Pending hardware |

Use the ESP8266 core API `ESP.getHeapStats(&freeHeap, &maxFreeBlock, &fragmentation)` at each sample point. Record bytes for the first two fields and percent for fragmentation. Remove or compile-gate temporary instrumentation after recording the numbers.

Stage 3 acceptance uses the same sampling points:

- Peak free-heap reduction versus this runtime baseline must not exceed 4 KiB
- The largest asset transfer must not reset the watchdog
- Free heap and largest block must recover after transfer
- The built image must keep at least 10% of the 1,044,464 B linked application ceiling free
- The OTA staging capacity must remain greater than the generated binary size

## Protocol replay baseline

Deterministic serial exchanges for OnStep 4.24, OnStepX 10.15/10.20/10.26/10.28, invalid version data, and disconnect are stored in `webui/fixtures/serial-replays.json`.

Each replay records raw serial replies including `#`. Firmware-facing parsers may strip the terminator before interpreting the value. The fixtures also declare expected normalized capabilities so UI tests never infer features from version strings.

## Existing HTTP compatibility boundary

Stage 1 freezes all existing `.htm`, `*-ajax.txt`, and `*-ajax-get.txt` routes. The current text protocol includes these forms:

- `id|value`
- `id&value`
- `selected` and `unselected`
- `enabled` and `disabled`

All action URLs remain owned by the firmware. The future Vue action adapter may select a semantic firmware endpoint, but it must never construct OnStep serial commands or reproduce firmware version checks.

## Reproducing the resource build

The repository does not yet commit a PlatformIO configuration. Reproduce with the `esp07s` board, ESP8266 Arduino Core 3.1.2, the 4M/1M linker layout, the current configuration headers, and `ArduinoOTA` as an explicit dependency. Record both the linker size report and the final binary size.
