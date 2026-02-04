#!/bin/bash
# Create a 512x512 icon with a blue border and "DW" text
magick -size 512x512 canvas:black \
       -fill "#000080" -draw "rectangle 20,20 492,492" \
       -fill white -pointsize 200 -gravity center -draw "text 0,0 'DW'" \
       app/src/main/res/mipmap-xxxhdpi/ic_launcher.png
