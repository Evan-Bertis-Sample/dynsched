#!/bin/bash

# decode_backtrace.sh
# Usage: ./decode_backtrace.sh <backtrace>
# Example: ./decode_backtrace.sh "Backtrace: 0x4008344d:0x3ffb1d30 0x400880e1:0x3ffb1d50 ..."

# Check if backtrace is provided
if [ $# -eq 0 ]; then
    echo "Usage: $0 <backtrace>"
    echo "Example: $0 \"Backtrace: 0x4008344d:0x3ffb1d30 ...\""
    exit 1
fi

# Get the backtrace string
BACKTRACE="$1"

# if this is a file, read the backtrace from the file
# otherwise, use the provided backtrace string

if [ -f "$BACKTRACE" ]; then
    BACKTRACE=$(cat "$BACKTRACE")
fi

# Extract addresses from the backtrace, by remove the "Backtrace: " prefix
# then split the string by spaces
ADDRESSES=$(echo "$BACKTRACE" | sed 's/Backtrace: //' | tr ' ' '\n')

# Check if addresses were extracted
if [ -z "$ADDRESSES" ]; then
    echo "Error: No addresses extracted from the backtrace."
    exit 1
fi

# Get the environment name from platformio.ini
ENVIRONMENT=$(grep 'default_envs' platformio.ini | awk -F ' = ' '{print $2}' | tr -d '[:space:]')
if [ -z "$ENVIRONMENT" ]; then
    # Default to esp32dev if default_envs is not set
    ENVIRONMENT="esp32dev"
fi

# Set the path to the ELF file
ELF_FILE=".pio/build/$ENVIRONMENT/firmware.elf"

# Check if the ELF file exists
if [ ! -f "$ELF_FILE" ]; then
    echo "Error: ELF file not found at $ELF_FILE"
    echo "Please build your project first."
    exit 1
fi

echo "Environment: $ENVIRONMENT"
echo "Decoding backtrace using $ELF_FILE..."
echo "Addresses: $ADDRESSES"

# Set the path to the xtensa-esp32-elf-addr2line utility
# Use PlatformIO's command to find the toolchain path
ADDR2LINE="xtensa-esp32-elf-addr2line"

echo ""

# Decode the addresses
echo "Decoding backtrace addresses..."
"$ADDR2LINE" -pfiaC -e "$ELF_FILE" $ADDRESSES
