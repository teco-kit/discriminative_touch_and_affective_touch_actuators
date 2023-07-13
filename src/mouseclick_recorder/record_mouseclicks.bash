#!/bin/bash

MOUSE_ID=$(xinput --list | grep -i -m 1 'PixArt USB Optical Mouse' | grep -o 'id=[0-9]\+' | grep -o '[0-9]\+')
RECORDING_FILE="mouserecording-$(date +%d_%m_%Y-%H_%M_%S)"
STATE1=$(xinput --query-state $MOUSE_ID | grep 'button\['"."'\]=down' | sort)

echo "timestamp;event;positionX;positionY" > data/$RECORDING_FILE

while true; do
        sleep 0.005
        timestamp=$(date +%s)
        STATE2=$(xinput --query-state $MOUSE_ID | grep 'button\['"."'\]=down' | sort)
        CLICK=$(comm -13 <(echo "$STATE1") <(echo "$STATE2"))
        if [[ -n $CLICK ]]; then
                valuatorZero=$(xinput --query-state $MOUSE_ID | grep 'valuator\[0\]')
                valuatorOne=$(xinput --query-state $MOUSE_ID | grep 'valuator\[1\]')
                echo "$timestamp;$CLICK;$valuatorZero;$valuatorOne" | xargs >> data/$RECORDING_FILE
                echo "$timestamp;$CLICK;$valuatorZero;$valuatorOne" | xargs
        fi
        STATE1=$STATE2
done
