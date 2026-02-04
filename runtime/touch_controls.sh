#!/bin/bash
# Generate a semi-transparent red "A" button
magick -size 200x200 canvas:none \
       -fill "rgba(255,0,0,0.5)" -draw "circle 100,100 100,10" \
       -fill white -pointsize 80 -gravity center -draw "text 0,0 'A'" \
       app/src/main/res/drawable/button_a.png
