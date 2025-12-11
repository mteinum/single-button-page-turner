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

1. Install required libraries:
   - M5Unified
   - ESP32 BLE Keyboard

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
