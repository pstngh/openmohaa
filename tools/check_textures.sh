#!/bin/bash
# Check which textures are missing a .jpg version
# Usage: ./check_textures.sh /path/to/game/textures

DIR="${1:-.}"

if [ ! -d "$DIR" ]; then
    echo "Directory not found: $DIR"
    echo "Usage: $0 /path/to/textures"
    exit 1
fi

missing_jpg=0
total=0
only_dds=0
only_tga=0

echo "Scanning: $DIR"
echo "==========================================="

# Find all unique texture base names (strip extension)
find "$DIR" -type f \( -iname "*.dds" -o -iname "*.tga" -o -iname "*.jpg" -o -iname "*.jpeg" -o -iname "*.png" -o -iname "*.bmp" -o -iname "*.pcx" \) \
    | sed 's/\.[^.]*$//' | sort -u | while read -r base; do

    total=$((total + 1))
    has_jpg=0
    has_tga=0
    has_dds=0
    has_png=0
    formats=""

    for ext in jpg jpeg; do
        if [ -f "${base}.${ext}" ] || [ -f "${base}.${ext^^}" ]; then
            has_jpg=1
            break
        fi
    done

    [ -f "${base}.tga" ] || [ -f "${base}.TGA" ] && has_tga=1
    [ -f "${base}.dds" ] || [ -f "${base}.DDS" ] && has_dds=1
    [ -f "${base}.png" ] || [ -f "${base}.PNG" ] && has_png=1

    # Build format list
    [ "$has_jpg" -eq 1 ] && formats="${formats}jpg "
    [ "$has_tga" -eq 1 ] && formats="${formats}tga "
    [ "$has_dds" -eq 1 ] && formats="${formats}dds "
    [ "$has_png" -eq 1 ] && formats="${formats}png "

    if [ "$has_jpg" -eq 0 ]; then
        relpath="${base#$DIR/}"
        if [ "$has_dds" -eq 1 ] && [ "$has_tga" -eq 0 ]; then
            echo "[DDS ONLY]  $relpath  (formats: $formats)"
        elif [ "$has_tga" -eq 1 ] && [ "$has_dds" -eq 0 ]; then
            echo "[TGA ONLY]  $relpath  (formats: $formats)"
        else
            echo "[NO JPG]    $relpath  (formats: $formats)"
        fi
    fi
done

# Summary using a second pass
echo ""
echo "==========================================="
echo "SUMMARY"
echo "==========================================="

total=$(find "$DIR" -type f \( -iname "*.dds" -o -iname "*.tga" -o -iname "*.jpg" -o -iname "*.jpeg" -o -iname "*.png" -o -iname "*.bmp" -o -iname "*.pcx" \) | sed 's/\.[^.]*$//' | sort -u | wc -l)

has_jpg_count=$(find "$DIR" -type f \( -iname "*.jpg" -o -iname "*.jpeg" \) | sed 's/\.[^.]*$//' | sort -u | wc -l)

has_tga_count=$(find "$DIR" -type f -iname "*.tga" | wc -l)
has_dds_count=$(find "$DIR" -type f -iname "*.dds" | wc -l)
has_png_count=$(find "$DIR" -type f -iname "*.png" | wc -l)

missing=$((total - has_jpg_count))

echo "Total unique textures:  $total"
echo "Have .jpg:              $has_jpg_count"
echo "Missing .jpg:           $missing"
echo ""
echo "File counts by format:"
echo "  .jpg files:  $has_jpg_count"
echo "  .tga files:  $has_tga_count"
echo "  .dds files:  $has_dds_count"
echo "  .png files:  $has_png_count"

if [ "$missing" -eq 0 ]; then
    echo ""
    echo "All textures have a .jpg version!"
else
    echo ""
    echo "$missing textures are MISSING a .jpg version."
fi
