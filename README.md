# m5cardputer-lora-mesh
M5Cardputer Mesh LoRaWAN Project

This repository contains an Arduino sketch for the M5Cardputer board, implementing a mesh network using LoRaWAN modules like the RAK for local message exchange and testing UART/AT commands in embedded systems. The project addresses compilation issues such as undeclared functions like 'updateMenuDisplay' and focuses on debugging LoRaWAN setups.

​
Table of Contents

    Project Description

    Hardware Requirements

    Software Requirements

    Installation

    Usage

    Repository Structure

    Contributing

    License

Project Description

This project enables mesh networking on the M5Cardputer with LoRaWAN integration for low-power, long-range communication. It includes features like voltage setting via setVoltage and menu updates with updateMenuDisplay, designed for experimentation in IoT and embedded development. The motivation is to simplify testing of AT commands over UART while resolving common build errors in Arduino environments.

​
Hardware Requirements

    M5Cardputer development board.

    LoRaWAN module (e.g., M5Stack Unit LoRaWAN-EU868).

    FTDI adapter for serial debugging and power cycling.

    Connections: Attach the LoRa module to Grove Port A, ensuring correct VCC, TX, and RX pins; swap TX/RX if messages fail to appear.

    ​

Software Requirements

    Arduino IDE version 2.x or later.

    Required libraries: M5Unified, LoRa, and Mesh (install via Library Manager in Arduino IDE).

    For mesh monitoring: Use the meshlog-tool available at anrijs.lv/meshlog.

    ​

Installation

    Clone the repository:

text
git clone https://github.com/yourusername/sketch_nov17a.git

Open Arduino IDE and install the listed libraries through the Library Manager.

Connect the LoRa module as described in Hardware Requirements.

Select the board "M5Cardputer" in Tools > Board, choose the correct port, and upload the sketch.
Verify setup by opening the Serial Monitor at 115200 baud.

    ​

Usage

    Power on the M5Cardputer and connect via Serial Monitor.

    Send AT commands to initialize LoRaWAN (e.g., for joining the network or sending test packets).

    For mesh networking, deploy multiple nodes and monitor exchanges using meshlog-tool; expect messages to propagate in the local network.

    Troubleshooting: If no output, perform a power cycle or check TX/RX wiring; common issues include baud rate mismatches or library version conflicts.
    Example Serial output:

    text
    LoRaWAN initialized. Ready for mesh transmission.

Add screenshots of wiring or Serial logs for clarity.

​
Repository Structure

    sketch_nov17a.ino: Main Arduino sketch with core functions like updateMenuDisplay and setVoltage.

    MeshCore/: Board variants for compatibility (e.g., xiao_rp2040/XiaoRP2040Board.h, xiao_nrf52/target.h).

    README.md: This documentation file.

    Additional files: Logs from git commits and file creations (e.g., variants for RP2040 and NRF52).

    ​

Contributing

Fork the repository and create a pull request for changes. Report issues via GitHub Issues for bugs like compilation errors or hardware incompatibilities. Follow standard practices: keep commits atomic and update this README with new features.

​
License

This project is licensed under the MIT License – see the LICENSE file for details. Credits: Built on M5Stack libraries; inspired by open-source Arduino mesh projects. Last Updated: November 17, 2025.
