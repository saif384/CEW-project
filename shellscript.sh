#!/bin/bash

# Compile C code
gcc Project.c cJSON.c report_email_sender.c -o weather_app -lcurl -lm

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful. Running the program after every 3 hours..."

    # Run the compiled program using watch every 30 seconds
    watch -n 10800 ./weather_app

else
    echo "Error: Compilation failed."
fi
