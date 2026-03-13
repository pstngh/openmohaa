# Convert an SVG file to a macOS .icns file at build time.
# Requires rsvg-convert (from librsvg) and iconutil (macOS built-in).
#
# Usage:
#   svg_to_icns(<svg_path> <output_icns_path> [TARGET <target>] [BACKGROUND <color>])
#
# If TARGET is specified, the conversion runs as a PRE_BUILD step.
# BACKGROUND sets the fill color behind the SVG (default: black).

function(svg_to_icns SVG_PATH OUTPUT_ICNS)
    cmake_parse_arguments(ARG "" "TARGET;BACKGROUND" "" ${ARGN})

    if(NOT ARG_BACKGROUND)
        set(ARG_BACKGROUND "black")
    endif()

    find_program(RSVG_CONVERT rsvg-convert)
    if(NOT RSVG_CONVERT)
        message(WARNING "rsvg-convert not found, cannot generate .icns from SVG. Install librsvg.")
        return()
    endif()

    find_program(ICONUTIL iconutil)
    if(NOT ICONUTIL)
        message(WARNING "iconutil not found, cannot generate .icns. This requires macOS.")
        return()
    endif()

    get_filename_component(ICNS_NAME ${OUTPUT_ICNS} NAME_WE)
    set(ICONSET_DIR "${CMAKE_BINARY_DIR}/${ICNS_NAME}.iconset")

    # macOS icon sizes: 16, 32, 128, 256, 512 (plus @2x variants)
    set(ICON_SIZES 16 32 128 256 512)

    set(CONVERT_COMMANDS "")

    foreach(SIZE IN LISTS ICON_SIZES)
        math(EXPR SIZE_2X "${SIZE} * 2")

        list(APPEND CONVERT_COMMANDS
            COMMAND ${RSVG_CONVERT} -b "${ARG_BACKGROUND}" -w ${SIZE} -h ${SIZE} "${SVG_PATH}" -o "${ICONSET_DIR}/icon_${SIZE}x${SIZE}.png"
            COMMAND ${RSVG_CONVERT} -b "${ARG_BACKGROUND}" -w ${SIZE_2X} -h ${SIZE_2X} "${SVG_PATH}" -o "${ICONSET_DIR}/icon_${SIZE}x${SIZE}@2x.png"
        )
    endforeach()

    if(ARG_TARGET)
        add_custom_command(TARGET ${ARG_TARGET} PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "${ICONSET_DIR}"
            ${CONVERT_COMMANDS}
            COMMAND ${ICONUTIL} -c icns -o "${OUTPUT_ICNS}" "${ICONSET_DIR}"
            COMMAND ${CMAKE_COMMAND} -E rm -rf "${ICONSET_DIR}"
            COMMENT "Generating ${ICNS_NAME}.icns from SVG"
        )
    else()
        file(MAKE_DIRECTORY "${ICONSET_DIR}")
        foreach(SIZE IN LISTS ICON_SIZES)
            math(EXPR SIZE_2X "${SIZE} * 2")
            execute_process(COMMAND ${RSVG_CONVERT} -b "${ARG_BACKGROUND}" -w ${SIZE} -h ${SIZE} "${SVG_PATH}" -o "${ICONSET_DIR}/icon_${SIZE}x${SIZE}.png")
            execute_process(COMMAND ${RSVG_CONVERT} -b "${ARG_BACKGROUND}" -w ${SIZE_2X} -h ${SIZE_2X} "${SVG_PATH}" -o "${ICONSET_DIR}/icon_${SIZE}x${SIZE}@2x.png")
        endforeach()
        execute_process(COMMAND ${ICONUTIL} -c icns -o "${OUTPUT_ICNS}" "${ICONSET_DIR}")
        file(REMOVE_RECURSE "${ICONSET_DIR}")
    endif()
endfunction()
