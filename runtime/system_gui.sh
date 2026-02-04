#!/bin/bash
# Create pill-shaped Start/Select buttons
magick -size 300x100 canvas:none \
    -fill "rgba(50,50,50,0.6)" -draw "roundrectangle 10,25 290,75 25,25" \
    -fill white -pointsize 40 -gravity center -draw "text 0,0 'START'" \
    app/src/main/res/drawable/btn_start.png

magick -size 300x100 canvas:none \
    -fill "rgba(50,50,50,0.6)" -draw "roundrectangle 10,25 290,75 25,25" \
    -fill white -pointsize 40 -gravity center -draw "text 0,0 'SELECT'" \
    app/src/main/res/drawable/btn_select.png
