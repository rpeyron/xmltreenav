#! /bin/sh

find  . -name '*.ico' | while read FILE
do
    NEWFILE=$(echo "$FILE" | sed 's:\([^/]*\)\.ico$:xpm_\1.xpm:')
    convert -resize 16x16 "$FILE" "$NEWFILE"
done
