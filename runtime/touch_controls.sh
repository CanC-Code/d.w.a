#!/bin/bash
# Ensure the output directory exists
mkdir -p app/src/main/res/drawable

# Base style: Semi-transparent red for A/B, Gray for Start/Select
# Generate 'A' Button
magick -size 200x200 canvas:none \
       -fill "rgba(200,0,0,0.6)" -draw "circle 100,100 100,10" \
       -fill white -pointsize 80 -font Arial -gravity center -draw "text 0,0 'A'" \
       app/src/main/res/drawable/button_a.png

# Generate 'B' Button
magick -size 200x200 canvas:none \
       -fill "rgba(200,0,0,0.6)" -draw "circle 100,100 100,10" \
       -fill white -pointsize 80 -font Arial -gravity center -draw "text 0,0 'B'" \
       app/src/main/res/drawable/button_b.png

# Generate D-Pad directions (Up, Down, Left, Right)
for dir in up down left right; do
    magick -size 150x150 canvas:none \
           -fill "rgba(50,50,50,0.6)" -draw "roundrectangle 0,0 150,150 20,20" \
           -fill white -pointsize 40 -gravity center -draw "text 0,0 '${dir^^}'" \
           app/src/main/res/drawable/button_$dir.png
done

# Generate Start/Select (Rectangular)
for btn in start select; do
    magick -size 240x80 canvas:none \
           -fill "rgba(100,100,100,0.6)" -draw "roundrectangle 0,0 240,80 10,10" \
           -fill white -pointsize 30 -gravity center -draw "text 0,0 '${btn^^}'" \
           app/src/main/res/drawable/button_$btn.png
done
