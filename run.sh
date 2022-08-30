#!/bin/sh

# if workspace_switcher file is not found, then run build.sh

if [ ! -f workspace_switcher ]; then
    echo "workspace_switcher file not found, running build.sh"
    ./build.sh rectangular
fi

event_id=$( cat /proc/bus/input/devices | grep -A4 "MX Master 3" | grep -oP '(?<=event)[0-9]+' )

if [ -z "$event_id" ]; then
    echo "Mx Master 3 not found"
    exit 1
fi

# run workspace_switcher
echo "Running workspace_switcher"
sudo ./workspace_switcher "/dev/input/event"$event_id