# M5Cardputer Mesh LoRaWAN Project

This repository contains an Arduino sketch for the M5Cardputer board, implementing a P2P LoRa mesh network for EU868 ISM (868.1 MHz) to send test messages like "Hello mescore test" hex to public loggers (e.g., anrijs.lv/meshlog). It handles AT commands via UART for RUI4.0.6 RAK3172 module, with robust SF12 params for 5km+ LOS range. Project resolves IDE issues like core dumps and undeclared functions (e.g., updateMenuDisplay). [web:780]

## Table of Contents
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Troubleshooting](#troubleshooting)
- [Repository Structure](#repository-structure)
- [Roadmap](#roadmap)
- [License](#license)

## Hardware Requirements
- M5Cardputer (ESP32S3-based).
- RAK3172 LoRa module (RUI4.0.6 EU868).
- FTDI adapter/M5Burner for UART debug (115200 baud, pins 1/2).
- Connections: LoRa to Grove Port A (TX/RX/VCC; swap TX/RX if no RX). Antenna: Internal Grove or external 868MHz 3dBi for outdoor LOS. [web:785]

## Software Requirements
- Arduino IDE 2.x+.
- Libraries: M5Unified (for M5Cardputer display/buttons), HardwareSerial (built-in).
- Tools: esptool.py (for erase_flash), M5Burner (logs), anrijs.lv/meshlog (EU868 monitor). [web:746]

## Installation
1. Clone:

git clone https://github.com/AtomDreams/m5cardputer-lora-mesh.git

2. Open `sketch_nov17a.ino` in Arduino IDE.
3. Board: "ESP32S3 Dev Module" > Partition: "Minimal SPIFFS" > Debug: "None".
4. Upload (hold BOOT, Ctrl+U). Erase first if core dump:

esptool.py --chip esp32s3 --port /dev/ttyACM0 erase_flash

5. Open Serial Monitor (115200 baud) for AT logs. [web:764][web:770]

## Usage
- Run sketch: Auto TX every 10s (hex "Hello mescore test" on 868.1 MHz SF12 BW125 CR4/5, PTP=14dBm).
- BtnA: Manual trigger TX.
- Monitor: M5 Display shows "Public TX OK!" or "RX: [decoded msg]".
- Meshlog: Post-TX, check https://anrijs.lv/meshlog/ (zoom Riga/Mārupe, filter freq 868100000 Hz / SF12 / BW125kHz / last 10min; search hex "48656C6C6F" for decoded msg, RSSI >-110dBm).
- Example code snippet (PSEND):

String hexMsg = "48656C6C6F206D6573636F72652074657374"; // "Hello mescore test"
probeCmd("AT+PSEND=" + hexMsg);

Outdoor LOS test: 100m+ clear south Riga, antenna up — expect +EVT:TXP2P DONE x12 in 2min. [web:746][web:747]

## Troubleshooting
- **Core Dump Error**: Erase flash (esptool.py), use Minimal SPIFFS/Debug None in IDE.
- **No TX/RX**: Check UART pins (1/2), baud 115200; power cycle. Swap TX/RX on LoRa.
- **No Meshlog Entry**: Indoor range <200m; outdoor LOS needed (5km+ SF12). Filter exact params; duty <1% EU. Alt: SF=7 for short test.
- **Compilation Fail**: Include M5Unified; verify ESP32S3 board package. [web:764][web:780]

## Repository Structure
- `sketch_nov17a.ino`: Main code (AT init, P2P setup, TX loop, hex decode).
- `MeshCore/variants/`: Board support (xiao_rp2040/*.h/cpp, xiao_nrf52/target.*, xiao_s3_wio/platformio.ini/target.*).
- `README.md`: This guide.
- Logs: Attached from M5Burner (TX success, no RX indoor). [web:872]

## Roadmap
- Add RX with Seeed T1000-E (PRECV 30s, hex decode SNR/RSSI).
- SF7 mode for indoor quick tests; alt freq 867550000.
- Full Meshtastic integration for multi-node mesh. Last Updated: Nov 17, 2025.

## License
MIT License — see LICENSE file. Credits: Based on M5Stack/RUI4 examples. [web:873]

