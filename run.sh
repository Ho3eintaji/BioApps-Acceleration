#!/bin/bash

# Store the current working directory
current_dir=$(pwd)

# Define the path to the 'sw' directory
sw_dir="sw/"

# Check if an argument was provided
if [ $# -lt 1 ]; then
    echo "Usage: $0 <app_name> [app_type]"
    exit 1
fi

# Store the app name from the argument
app_name="$1"

# Determine the app type from the second argument, default to the original path
if [ $# -ge 2 ] && [ "$2" == "power_apps" ]; then
    app_path="applications/power_apps/$app_name/$app_name.flash_load.hex"
    hex_file="$current_dir/sw/applications/power_apps/$app_name/$app_name.flash_load.hex"
    echo "2"
elif [ $# -ge 2 ] && [ "$2" == "x_applications" ]; then
    app_path="x_applications/$app_name/$app_name.flash_load.hex"
    hex_file="$current_dir/sw/x_applications/$app_name/$app_name.flash_load.hex"
    echo "3"
else
    app_path="applications/$app_name/$app_name.flash_load.hex"
    hex_file="$current_dir/sw/applications/$app_name/$app_name.flash_load.hex"
    echo "1"
fi

# Change directory to 'sw'
cd "$sw_dir"

# Run the 'make' command with the determined app path
make clean "$app_path" TARGET=testing_board_use_fll

# Change back to the previous directory
cd "$current_dir"

# Define the path to the iceprog directory
iceprog_dir="hw/vendor/esl_epfl_x_heep/sw/vendor/yosyshq_icestorm/iceprog"

# Flash load using iceprog
cd "$iceprog_dir"
make clean
make all

# Run iceprog with the full path to the hex file and return to the previous directory
./iceprog -d i:0x0403:0x6011 -I B "$hex_file"

cd "$current_dir"

# Start picocom to monitor the result
picocom -b 9600 -r -l --imap lfcrlf /dev/ttyUSB2

# Optionally, you can print a message to indicate the script has finished
echo "Script completed."

# #!/bin/bash

# # Store the current working directory
# current_dir=$(pwd)

# # Define the path to the 'sw' directory
# sw_dir="sw/"

# # Check if an argument was provided
# if [ $# -lt 1 ]; then
#     echo "Usage: $0 <app_name> [app_type]"
#     exit 1
# fi

# # Store the app name from the argument
# app_name="$1"

# # Run the 'make' command with the provided app name
# # Determine the app type from the second argument, default to the original path
# if [ $# -ge 2 ] && [ "$2" == "power_apps" ]; then
#     app_path="applications/power_apps/$app_name/$app_name.flash_load.hex"
#     echo "2"
# else
#     app_path="applications/$app_name/$app_name.flash_load.hex"
#     echo "1"
# fi
# # make clean "applications/$app_name/$app_name.flash_load.hex" TARGET=testing_board_use_fll

# # Change directory to 'sw'
# cd "$sw_dir"

# # Run the 'make' command with the determined app path
# make clean "$app_path" TARGET=testing_board_use_fll

# # Change back to the previous directory
# cd "$current_dir"

# # Define the path to the iceprog directory
# iceprog_dir="hw/vendor/esl_epfl_x_heep/sw/vendor/yosyshq_icestorm/iceprog"

# # Flash load using iceprog
# cd "$iceprog_dir"
# make clean
# make all

# # Run iceprog with the full path to the hex file and return to the previous directory
# if [ -z "$2" ] || [ "$2" != "power_apps" ]; then
#     hex_file="$current_dir/sw/applications/$app_name/$app_name.flash_load.hex"
# else
#     hex_file="$current_dir/sw/applications/power_apps/$app_name/$app_name.flash_load.hex"
# fi

# ./iceprog -d i:0x0403:0x6011 -I B "$hex_file"

# cd "$current_dir"

# # Start picocom to monitor the result
# picocom -b 9600 -r -l --imap lfcrlf /dev/ttyUSB2

# # Optionally, you can print a message to indicate the script has finished
# echo "Script completed."
