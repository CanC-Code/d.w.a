# Generate Standard Icon
magick -size 512x512 canvas:black \
       -fill "#000080" -draw "rectangle 20,20 492,492" \
       -fill white -pointsize 200 -gravity center -draw "text 0,0 'DW'" \
       app/src/main/res/mipmap-xxxhdpi/ic_launcher.png

# Generate Round Icon (To satisfy the manifest error)
magick -size 512x512 canvas:none \
       -fill black -draw "circle 256,256 256,20" \
       -fill "#000080" -draw "circle 256,256 256,40" \
       -fill white -pointsize 200 -gravity center -draw "text 0,0 'DW'" \
       app/src/main/res/mipmap-xxxhdpi/ic_launcher_round.png
