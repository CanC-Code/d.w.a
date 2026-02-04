#!/bin/bash
# Define colors for NES-style
BTN_COLOR="rgba(30,30,30,0.7)"
ACTION_COLOR="rgba(180,0,0,0.8)"
TEXT_COLOR="rgba(255,255,255,0.9)"

# Ensure the output directory exists
mkdir -p app/src/main/res/drawable

# 1. Create D-PAD Buttons (Generates BOTH button_up and dpad_up)
create_arrow() {
    local name=$1
    local rotation=$2
    # This creates a square button with a rotating arrow inside
    magick -size 200x200 canvas:none \
        -fill "$BTN_COLOR" -draw "roundrectangle 10,10 190,190 20,20" \
        -fill "$TEXT_COLOR" -draw "push graphic-context translate 100,100 rotate $rotation polygon 0,-60 60,40 -60,40 pop" \
        "app/src/main/res/drawable/dpad_$name.png"
    
    # Create a symlink or copy to satisfy both naming styles
    cp "app/src/main/res/drawable/dpad_$name.png" "app/src/main/res/drawable/button_$name.png"
}

create_arrow "up" 0
create_arrow "right" 90
create_arrow "down" 180
create_arrow "left" 270

# 2. Create Action Buttons (A and B)
for btn in a b; do
    magick -size 200x200 canvas:none \
        -fill "$ACTION_COLOR" -draw "circle 100,100 100,10" \
        -fill "$TEXT_COLOR" -pointsize 100 -gravity center -draw "text 0,0 '${btn^^}'" \
        "app/src/main/res/drawable/button_$btn.png"
done

# 3. Create Menu Buttons (Start and Select)
for btn in start select; do
    magick -size 300x100 canvas:none \
        -fill "$BTN_COLOR" -draw "roundrectangle 10,10 290,90 15,15" \
        -fill "$TEXT_COLOR" -pointsize 40 -gravity center -draw "text 0,0 '${btn^^}'" \
        "app/src/main/res/drawable/button_$btn.png"
done

echo "GUI Assets generated: dpad_up, dpad_down, dpad_left, dpad_right, button_a, button_b."
