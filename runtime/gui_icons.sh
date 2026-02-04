#!/bin/bash
# Define colors for NES-style (Dark Grey/Black with transparency)
BTN_COLOR="rgba(30,30,30,0.7)"
ARROW_COLOR="rgba(200,200,200,0.8)"

# Ensure the output directory exists
mkdir -p app/src/main/res/drawable

# Function to create an arrow button
create_arrow() {
    local name=$1
    local rotation=$2
    
    # Create a square button with a triangle arrow
    magick -size 200x200 canvas:none \
        -fill "$BTN_COLOR" -draw "roundrectangle 10,10 190,190 20,20" \
        -fill "$ARROW_COLOR" -draw "rotate $rotation polygon 100,40 160,140 40,140" \
        "app/src/main/res/drawable/dpad_$name.png"
}

# Generate the 4 directions
create_arrow "up" 0
create_arrow "right" 90
create_arrow "down" 180
create_arrow "left" 270
