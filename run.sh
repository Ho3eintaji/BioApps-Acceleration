#!/bin/bash

# Store the current working directory
current_dir=$(pwd)

# Define the path to the 'sw' directory
sw_dir="sw/"

# Check if an argument was provided
if [ $# -ne 1 ]; then
    echo "Usage: $0 <app_name>"
    exit 1
fi

# Store the app name from the argument
app_name="$1"

# Change directory to 'sw'
cd "$sw_dir"

# Run the 'make' command with the provided app name
make clean "applications/$app_name/$app_name.flash_load.hex" TARGET=testing_board_use_fll

# Change back to the previous directory
cd "$current_dir"

# Define the path to the iceprog directory
iceprog_dir="hw/vendor/esl_epfl_x_heep/sw/vendor/yosyshq_icestorm/iceprog"

# Flash load using iceprog
cd "$iceprog_dir"
make clean
make all

# Run iceprog with the full path to the hex file and return to the previous directory
./iceprog -d i:0x0403:0x6011 -I B "$current_dir/sw/applications/$app_name/$app_name.flash_load.hex"
cd "$current_dir"

# Start picocom to monitor the result
picocom -b 9600 -r -l --imap lfcrlf /dev/ttyUSB2

# Optionally, you can print a message to indicate the script has finished
echo "Script completed."
