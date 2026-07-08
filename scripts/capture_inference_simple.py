#!/usr/bin/env python3
"""
Simple script to capture Raspberry Pi Pico serial output to a timestamped log file.
Usage: python3 capture_inference_simple.py [port] [baudrate]
"""

import serial
import sys
from datetime import datetime
import os
import glob
import time

def find_serial_port():
    """Try to find Raspberry Pi Pico port automatically"""
    ports = glob.glob('/dev/cu.usbmodem*') + glob.glob('/dev/tty.usbmodem*') + glob.glob('/dev/cu.usbserial*') + glob.glob('/dev/tty.usbserial*')
    if ports:
        return ports[0]
    return None

def main():
    port = sys.argv[1] if len(sys.argv) > 1 else find_serial_port()
    baudrate = int(sys.argv[2]) if len(sys.argv) > 2 else 115200
    
    if port is None:
        print("❌ Could not find serial port automatically.")
        print("\nUsage: python3 capture_inference_simple.py <port> [baudrate]")
        sys.exit(1)

    try:
        # For Raspberry Pi Pico, USB CDC explicitly requires setting DTR to True
        ser = serial.Serial()
        ser.port = port
        ser.baudrate = baudrate
        ser.timeout = 0.1
        
        # Open port
        ser.open()
        ser.dtr = True # CRITICAL for Pico
        ser.rts = True # Sometimes also required
        
        # Pico 2W often needs DTR toggled to trigger a soft reboot / flush
        # when connected so the sketch restarts and hits the while(!Serial) block
        ser.dtr = False
        time.sleep(0.05)
        ser.dtr = True
        
    except serial.SerialException as e:
        print(f"\n❌ Serial Error: {e}")
        print(f"\nAvailable ports might be:")
        for p in glob.glob('/dev/cu.*') + glob.glob('/dev/tty.*'):
            if 'usb' in p.lower():
                print(f"  - {p}")
        sys.exit(1)

    os.makedirs("logs", exist_ok=True)
    ts = datetime.now().strftime("%Y%m%d_%H%M%S")
    output_file = os.path.join("logs", f"pico_serial_log_{ts}.txt")

    print(f"📡 Connected to {port} at {baudrate} baud...")
    print(f"📝 Logging to: {output_file}")
    print("Press Ctrl+C to stop.")

    with open(output_file, 'w', buffering=1) as f:
        header = f"=== Raspberry Pi Pico Serial Log ===\n"
        header += f"Port: {port}\n"
        header += f"Baudrate: {baudrate}\n"
        header += f"Started: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n"
        header += "=" * 50 + "\n\n"
        f.write(header)
        print(header, end="")
        
        try:
            while True:
                # readline will block up to our timeout
                line = ser.readline()
                if line:
                    try:
                        decoded_line = line.decode('utf-8', errors='replace').rstrip()
                        timestamp = datetime.now().strftime('%H:%M:%S.%f')[:-3]
                        log_line = f"[{timestamp}] {decoded_line}"
                        f.write(log_line + "\n")
                        print(log_line)
                    except Exception as e:
                        print(f"Error decoding line: {e}")
        except KeyboardInterrupt:
            print("\n✓ Stopped.")
        finally:
            if ser.is_open:
                ser.close()

if __name__ == "__main__":
    main()
