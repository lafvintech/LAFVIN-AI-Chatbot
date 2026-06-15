#!/usr/bin/env python3
"""
LAFVIN ESP32-S3 AI Chatbot Flash Tool (macOS)
Auto-detects serial port, handles errors gracefully.
"""

import os
import sys
import glob
import subprocess
import time

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
ESPTOOL = os.path.join(SCRIPT_DIR, "esptool", "esptool.py")

FIRMWARES = {
    "1": ("Xiaozhi", "Xiaozhi.bin"),
    "2": ("ChatGPT", "ChatGPT.bin"),
}


def find_port():
    """Auto-detect the ESP32 serial port."""
    # Priority order: usbserial > usbmodem > others
    candidates = (
        glob.glob("/dev/cu.usbserial-*")
        + glob.glob("/dev/cu.SLAB_USBtoUART*")
        + glob.glob("/dev/cu.wchusbserial*")
        + glob.glob("/dev/cu.usbmodem*")
    )
    # Filter out known non-ESP ports
    skip = {"debug-console", "Bluetooth", "wlan"}
    candidates = [p for p in candidates if not any(s in p for s in skip)]
    return candidates


def run_esptool(args, capture=False):
    """Run esptool with auto-detected or specified port."""
    cmd = [sys.executable, ESPTOOL] + args
    if capture:
        return subprocess.run(cmd, capture_output=True, text=True)
    return subprocess.run(cmd)


def detect_chip(port):
    """Try to connect and detect chip type."""
    result = run_esptool(["--port", port, "--chip", "esp32s3", "chip-id"], capture=True)
    return result.returncode == 0


def erase_flash(port):
    print(f"\n🗑️  Erasing flash on {port}...")
    result = run_esptool(["--port", port, "--chip", "esp32s3", "erase-flash"])
    return result.returncode == 0


def write_flash(port, firmware_path):
    print(f"\n⚡ Flashing {os.path.basename(firmware_path)} → {port}...")
    result = run_esptool([
        "--port", port,
        "--chip", "esp32s3",
        "--baud", "460800",
        "write-flash", "-z", "0x0",
        firmware_path,
    ])
    return result.returncode == 0


def main():
    print("=" * 50)
    print("  LAFVIN ESP32-S3 AI Chatbot Flash Tool")
    print("=" * 50)

    # Check esptool exists
    if not os.path.exists(ESPTOOL):
        # Try system esptool
        result = subprocess.run(
            [sys.executable, "-m", "esptool", "--version"],
            capture_output=True, text=True,
        )
        if result.returncode == 0:
            # Use module version
            global ESPTOOL
            ESPTOOL = None
        else:
            print("\n❌ esptool not found!")
            print("   Install with: pip install esptool")
            sys.exit(1)

    # Firmware selection
    print("\nSelect firmware:")
    for key, (name, _) in FIRMWARES.items():
        fw_path = os.path.join(SCRIPT_DIR, FIRMWARES[key][1])
        size = f"{os.path.getsize(fw_path) // 1024 // 1024}MB" if os.path.exists(fw_path) else "missing"
        print(f"  {key}. {name} ({size})")

    choice = input("\nEnter choice (1 or 2): ").strip()
    if choice not in FIRMWARES:
        print("❌ Invalid choice.")
        sys.exit(1)

    fw_name, fw_file = FIRMWARES[choice]
    fw_path = os.path.join(SCRIPT_DIR, fw_file)

    if not os.path.exists(fw_path):
        print(f"❌ Firmware file not found: {fw_path}")
        sys.exit(1)

    # Boot mode instructions
    print(f"\n📋 Flashing: {fw_name}")
    print("\n⚠️  Put ESP32 in BOOT mode:")
    print("   1. Hold the BOOT button")
    print("   2. Plug in USB cable")
    print("   3. Release BOOT button")
    print("   4. Press Enter here\n")
    input("Press Enter when ready...")

    # Auto-detect port
    ports = find_port()
    if not ports:
        print("\n❌ No ESP32 port found!")
        print("   Make sure you held BOOT while connecting.")
        print("   Available ports: " + str(glob.glob("/dev/cu.*")))
        sys.exit(1)

    if len(ports) == 1:
        port = ports[0]
        print(f"\n✅ Found port: {port}")
    else:
        print(f"\nMultiple ports found:")
        for i, p in enumerate(ports):
            print(f"  {i+1}. {p}")
        idx = input("Select port (1): ").strip() or "1"
        port = ports[int(idx) - 1]

    # Flash
    print(f"\n🚀 Starting flash...")
    if not erase_flash(port):
        print("\n❌ Erase failed! Check BOOT mode and try again.")
        sys.exit(1)

    if not write_flash(port, fw_path):
        print("\n❌ Flash failed!")
        sys.exit(1)

    print(f"\n✅ {fw_name} flashed successfully!")
    print("   Press RST button to restart the device.\n")


if __name__ == "__main__":
    main()
