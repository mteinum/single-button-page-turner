# Kobo Page Turner

[![Arduino Build](https://github.com/mteinum/kobo-page-turner/actions/workflows/build.yml/badge.svg)](https://github.com/mteinum/kobo-page-turner/actions/workflows/build.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Platform](https://img.shields.io/badge/platform-ESP32-blue.svg)](https://www.espressif.com/en/products/socs/esp32)

A BLE keyboard controller for M5Stack AtomS3 that enables wireless page turning on Kobo e-readers.

![Kobo Page Turner](docs/IMG_8010.jpeg)

## Author

Morten Teinum (morten.teinum@gmail.com)

## Hardware

- **Device**: M5Stack AtomS3
- **Connection**: Bluetooth Low Energy (BLE)

## Features

- **Short Press**: Next page (Right Arrow key)
- **Long Press** (≥600ms): Previous page (Left Arrow key)
- **Visual Status Indicator**: 
  - Green filled square: BLE connected
  - Red outline square: BLE disconnected

## Setup

### Method 1: Pre-compiled Firmware (Easiest)

Download the latest firmware from [Releases](https://github.com/mteinum/kobo-page-turner/releases)

#### Using esptool (Command Line)
```bash
pip install esptool

# Flash all three binary files
esptool.py --chip esp32s3 --port /dev/ttyUSB0 --baud 921600 write_flash -z \
  0x0 kobo-page-turner.ino.bootloader.bin \
  0x8000 kobo-page-turner.ino.partitions.bin \
  0x10000 kobo-page-turner.ino.bin
```

**Note**: Replace `/dev/ttyUSB0` with your port:
- Linux: `/dev/ttyUSB0` or `/dev/ttyACM0`
- macOS: `/dev/cu.usbserial-*` or `/dev/cu.wchusbserial*`
- Windows: `COM3`, `COM4`, etc.

#### Using ESP Flash Download Tool (Windows)
1. Download [Espressif Flash Download Tool](https://www.espressif.com/en/support/download/other-tools)
2. Select ESP32-S3
3. Load the binary files:
   - `0x0` → `kobo-page-turner.ino.bootloader.bin`
   - `0x8000` → `kobo-page-turner.ino.partitions.bin`
   - `0x10000` → `kobo-page-turner.ino.bin`
4. Select your COM port and click "Start"

### Method 2: Build from Source

#### Prerequisites

1. Install Arduino IDE or Arduino CLI
2. Install ESP32 board support
3. Install required libraries:
   - [M5Unified](https://github.com/m5stack/M5Unified)
   - [ESP32 BLE Keyboard](https://github.com/T-vK/ESP32-BLE-Keyboard)

### ESP32 BLE Keyboard Library Modifications

The project requires a modified version of the ESP32 BLE Keyboard library (v0.3.0) to enhance BLE security and compatibility with Kobo e-readers.

#### Applying the Patch

A patch file is provided in `patches/BleKeyboard.patch`. After installing the ESP32 BLE Keyboard library:

```bash
cd ~/Arduino/libraries/ESP32-BLE-Keyboard
patch -p1 < /path/to/kobo-page-turner/patches/BleKeyboard.patch
```

#### Key Modifications

The patch makes the following changes:
- **Enhanced BLE Security**: Upgrades authentication to `ESP_LE_AUTH_REQ_SC_MITM_BOND` for better pairing security
- **NimBLE Support**: Adds conditional compilation for NimBLE stack
- **String Compatibility**: Fixes string initialization for device name and manufacturer
- **Configurable Device IDs**: Adds methods to set vendor ID, product ID, and version
- **Notification Management**: Properly handles BLE descriptor notifications

<details>
<summary>View complete patch content</summary>

The patch file is located at `patches/BleKeyboard.patch` in this repository. It contains all necessary modifications to make the ESP32 BLE Keyboard library compatible with Kobo e-readers.

</details>


### Upload Instructions

2. Upload the sketch to your M5Stack AtomS3

3. Pair with your Kobo e-reader:
   - Device name: `Kobo PageTurner`
   - Look for it in Bluetooth settings on your Kobo

## Usage

Once paired, simply press the button on the AtomS3:
- **Quick tap**: Turn to next page
- **Press and hold**: Go back to previous page

The display shows the current connection status and button functions.

## Configuration

You can modify these settings in the code:

- `BLE_DEVICE_NAME`: Change the Bluetooth device name
- `LONG_PRESS_MS`: Adjust the long press threshold (default: 600ms)

## License

MIT License - see [LICENSE](LICENSE) file for details.
