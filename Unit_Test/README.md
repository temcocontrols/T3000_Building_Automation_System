# Firmware update unit tests

Host tests for the T3000 ISP tool and the ESP bootloader firmware update. Production sources are not modified. Each run copies `isp_device_supports_resume()` out of `ISP/global_function.cpp` and compiles that copy with the macros from the production headers.

The suite covers:

- Resume-capable devices: bootloader 48+ on product 88, and bootloader 47 on Tstat10, Tstat11, and Hub.
- Older firmware that cannot resume: full update from byte 0, with no read of the saved-update registers.
- Wi-Fi/TFTP, serial bin, serial hex, and the TCP pre-check.
- Bootloader fresh OTA, raw resume, 4 KB rewind, classic `0x7F` / `0x3F` / `0x1F` on an old device, and finish/clear.
- ISP helpers the flash application calls: hex conversion, hex checksum, product and firmware names, and the bootloader-must-update decision. Dialog, socket, and Modbus I/O functions are left out. They need the Windows UI and a device. Unused functions are not included.

From this folder:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File build_and_run.ps1
```

The script uses the same `gcc` that builds the Hub unit tests (`%TEMP%\w64devkit\w64devkit\bin\gcc.exe`). The executable is `build/run_firmware_update_tests.exe`.

GitHub Actions runs the same suite on every push and on every pull request, including after the pull request is merged. The workflow is `.github/workflows/unit_test.yml`. On the runner the bootloader tree is not part of this repository, so the ISP checks always run and the bootloader source lock runs when `Bootloader/T3_Bootloader` is present beside the checkout.
