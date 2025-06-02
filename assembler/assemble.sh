#!/bin/bash

# Check input
if [ "$#" -ne 1 ]; then
    echo "Usage: ./assemble <file.asm>"
    exit 1
fi

ASM_FILE="$1"

# Check if file exists
if [ ! -f "$ASM_FILE" ]; then
    echo "Error: File '$ASM_FILE' not found."
    exit 1
fi

# Extract base filename without extension
BASENAME=$(basename "$ASM_FILE" .asm)
RAW_BIN="${BASENAME}.bin"
FINAL_BIN="${BASENAME}_final.bin"

# Assemble using customasm
customasm "$ASM_FILE"
if [ $? -ne 0 ]; then
    echo "customasm failed to assemble $ASM_FILE"
    exit 1
fi

# Check if .bin was produced
if [ ! -f "$RAW_BIN" ]; then
    echo "Expected output file '$RAW_BIN' not found. Check your .asm's 'out' directive."
    exit 1
fi

# Python: Prepend 16-byte name and overwrite original .bin
python3 - <<EOF
import os

input_file = "$RAW_BIN"
base_name = os.path.basename(input_file)
name_without_ext = os.path.splitext(base_name)[0]
encoded_name = name_without_ext.encode('ascii', errors='ignore')[:16]
padded_name = encoded_name.ljust(16, b'\0')

with open(input_file, 'rb') as f:
    data = f.read()

with open("$FINAL_BIN", 'wb') as f:
    f.write(padded_name + data)

print("Output written to:", "$FINAL_BIN")
EOF

# Cleanup
rm -f "$RAW_BIN"

