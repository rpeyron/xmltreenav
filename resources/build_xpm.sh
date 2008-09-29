#! /bin/sh

find  . -name '*.ico' | while read FILE
do
    NEWFILE=$(echo "$FILE" | sed 's:\([^/]*\)\.ico$:xpm_\1.xpm:')
    convert -resize 16x16 "$FILE" "$NEWFILE"
done

perl -pi -w -e 's/static char/static const char/g;' *.xpm
