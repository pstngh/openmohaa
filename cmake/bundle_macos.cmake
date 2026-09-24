if(NOT APPLE OR NOT BUILD_MACOS_BUNDLE)
    return()
endif()

if(NOT BUILD_MACOS_APP)
    message(FATAL_ERROR "BUILD_MACOS_BUNDLE requires BUILD_MACOS_APP=ON")
endif()

include(utils/set_output_dirs)

set(BUNDLE_NAME mohbots)
set(BUNDLE_MACOS_SUBDIR ${BUNDLE_NAME}.app/Contents/MacOS)

# Replaces finish_macos_app: builds the client as mohbots.app and places the
# renderers and game modules inside it, so the bundle is self-contained
function(create_macos_bundle)
    set_target_properties(${CLIENT_BINARY} PROPERTIES OUTPUT_NAME ${BUNDLE_NAME})

    # Keep configs, logs, and saves in the game directory
    target_compile_definitions(${CLIENT_BINARY} PRIVATE PORTABLE_INSTALL)

    set(BUNDLE_TARGETS)
    if(USE_RENDERER_DLOPEN AND BUILD_RENDERER_GL1)
        list(APPEND BUNDLE_TARGETS ${RENDERER_GL1_BINARY})
    endif()
    if(USE_RENDERER_DLOPEN AND BUILD_RENDERER_GL2)
        list(APPEND BUNDLE_TARGETS ${RENDERER_GL2_BINARY})
    endif()
    if(BUILD_GAME_LIBRARIES)
        list(APPEND BUNDLE_TARGETS ${CGAME_MODULE_BINARY_BASEGAME} ${GAME_MODULE_BINARY_BASEGAME})
    endif()
    foreach(TARGET IN LISTS BUNDLE_TARGETS)
        set_output_dirs(${TARGET} SUBDIRECTORY ${BUNDLE_MACOS_SUBDIR})
        add_dependencies(${CLIENT_BINARY} ${TARGET})
    endforeach()

    # Render the icon from the SVG when librsvg is installed, since the
    # committed .icns stops at 128x128
    find_program(RSVG_CONVERT rsvg-convert)
    find_program(ICONUTIL iconutil)
    if(RSVG_CONVERT AND ICONUTIL)
        set(BUNDLE_ICON_PATH ${CMAKE_BINARY_DIR}/${BUNDLE_NAME}.icns)
        set(ICONSET_DIR ${CMAKE_BINARY_DIR}/${BUNDLE_NAME}.iconset)
        set(SVG_PATH ${CMAKE_SOURCE_DIR}/misc/openmohaa.svg)
        set(RENDER_COMMANDS)
        foreach(SIZE IN ITEMS 16 32 128 256 512)
            math(EXPR SIZE_2X "${SIZE} * 2")
            list(APPEND RENDER_COMMANDS
                COMMAND ${RSVG_CONVERT} -b black -w ${SIZE} -h ${SIZE} ${SVG_PATH} -o ${ICONSET_DIR}/icon_${SIZE}x${SIZE}.png
                COMMAND ${RSVG_CONVERT} -b black -w ${SIZE_2X} -h ${SIZE_2X} ${SVG_PATH} -o ${ICONSET_DIR}/icon_${SIZE}x${SIZE}@2x.png)
        endforeach()
        add_custom_command(TARGET ${CLIENT_BINARY} PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory ${ICONSET_DIR}
            ${RENDER_COMMANDS}
            COMMAND ${ICONUTIL} -c icns -o ${BUNDLE_ICON_PATH} ${ICONSET_DIR}
            COMMAND ${CMAKE_COMMAND} -E rm -rf ${ICONSET_DIR}
            COMMENT "Generating ${BUNDLE_NAME}.icns"
            VERBATIM)
    else()
        set(BUNDLE_ICON_PATH ${MACOS_ICON_PATH})
    endif()

    set(RESOURCES_DIR $<TARGET_FILE_DIR:${CLIENT_BINARY}>/../Resources)
    add_custom_command(TARGET ${CLIENT_BINARY} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${RESOURCES_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy ${BUNDLE_ICON_PATH} ${RESOURCES_DIR})

    # No URL scheme: the separate openmohaa client keeps handling openmohaa:// links
    set(MACOS_APP_BUNDLE_NAME ${BUNDLE_NAME})
    set(MACOS_APP_EXECUTABLE_NAME ${BUNDLE_NAME})
    set(MACOS_APP_GUI_IDENTIFIER org.openmohaa.${BUNDLE_NAME})
    get_filename_component(MACOS_APP_ICON_FILE ${BUNDLE_ICON_PATH} NAME)
    set(MACOS_APP_SHORT_VERSION_STRING ${PRODUCT_VERSION})
    set(MACOS_APP_BUNDLE_VERSION ${PRODUCT_VERSION})
    set(MACOS_APP_DEPLOYMENT_TARGET ${CMAKE_OSX_DEPLOYMENT_TARGET})
    set(MACOS_APP_COPYRIGHT ${COPYRIGHT})
    set(MACOS_APP_PLIST_URL_TYPES "")

    configure_file(${CMAKE_SOURCE_DIR}/cmake/Info.plist.in
        ${CMAKE_BINARY_DIR}/${BUNDLE_NAME}-Info.plist @ONLY)

    set_target_properties(${CLIENT_BINARY} PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_BINARY_DIR}/${BUNDLE_NAME}-Info.plist)
endfunction()

list(REMOVE_ITEM POST_CONFIGURE_FUNCTIONS finish_macos_app)
list(APPEND POST_CONFIGURE_FUNCTIONS create_macos_bundle)
