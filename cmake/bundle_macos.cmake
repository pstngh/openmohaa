if(NOT APPLE OR NOT BUILD_MACOS_BUNDLE)
    return()
endif()

# BUILD_MACOS_BUNDLE requires BUILD_MACOS_APP (MACOSX_BUNDLE on the client target)
if(NOT BUILD_MACOS_APP)
    message(FATAL_ERROR "BUILD_MACOS_BUNDLE requires BUILD_MACOS_APP=ON")
endif()

include(utils/set_output_dirs)
include(utils/svg_to_icns)

set(BUNDLE_NAME "mohbots")
set(BUNDLE_MACOS_SUBDIR "${BUNDLE_NAME}.app/Contents/MacOS")
set(BUNDLE_ICON_SVG "${CMAKE_SOURCE_DIR}/misc/openmohaa.svg")

function(create_macos_bundle)
    # Rename the .app bundle from openmohaa.app to mohbots.app
    set_target_properties(${CLIENT_BINARY} PROPERTIES OUTPUT_NAME ${BUNDLE_NAME})

    # Portable install: configs/logs/saves go to the game directory
    # instead of ~/Library/Application Support/
    target_compile_definitions(${CLIENT_BINARY} PRIVATE PORTABLE_INSTALL)

    # Clear INSTALL_DESTINATION so installer.cmake skips this target
    # (the explicit install in client.cmake still handles it)
    set_target_properties(${CLIENT_BINARY} PROPERTIES INSTALL_DESTINATION "")

    # Redirect renderer output dirs to match the renamed .app
    # (finish_macos_app set these to openmohaa.app/Contents/MacOS — override them)
    if(USE_RENDERER_DLOPEN AND BUILD_RENDERER_GL1)
        set_output_dirs(${RENDERER_GL1_BINARY} SUBDIRECTORY ${BUNDLE_MACOS_SUBDIR})
        set_target_properties(${RENDERER_GL1_BINARY} PROPERTIES INSTALL_DESTINATION "")
        add_dependencies(${CLIENT_BINARY} ${RENDERER_GL1_BINARY})
    endif()

    if(USE_RENDERER_DLOPEN AND BUILD_RENDERER_GL2)
        set_output_dirs(${RENDERER_GL2_BINARY} SUBDIRECTORY ${BUNDLE_MACOS_SUBDIR})
        set_target_properties(${RENDERER_GL2_BINARY} PROPERTIES INSTALL_DESTINATION "")
        add_dependencies(${CLIENT_BINARY} ${RENDERER_GL2_BINARY})
    endif()

    # Place game modules inside the .app bundle
    if(BUILD_GAME_LIBRARIES)
        set_output_dirs(${CGAME_MODULE_BINARY_BASEGAME} SUBDIRECTORY ${BUNDLE_MACOS_SUBDIR})
        set_target_properties(${CGAME_MODULE_BINARY_BASEGAME} PROPERTIES INSTALL_DESTINATION "")
        set_output_dirs(${GAME_MODULE_BINARY_BASEGAME} SUBDIRECTORY ${BUNDLE_MACOS_SUBDIR})
        set_target_properties(${GAME_MODULE_BINARY_BASEGAME} PROPERTIES INSTALL_DESTINATION "")
        add_dependencies(${CLIENT_BINARY} ${CGAME_MODULE_BINARY_BASEGAME} ${GAME_MODULE_BINARY_BASEGAME})
    endif()

    # Generate the .icns icon at build time and copy it into the bundle.
    # Fall back to the committed icon when librsvg isn't installed.
    find_program(RSVG_CONVERT rsvg-convert)
    find_program(ICONUTIL iconutil)
    if(RSVG_CONVERT AND ICONUTIL)
        set(BUNDLE_ICON_PATH "${CMAKE_BINARY_DIR}/${BUNDLE_NAME}.icns")
        svg_to_icns("${BUNDLE_ICON_SVG}" "${BUNDLE_ICON_PATH}" TARGET ${CLIENT_BINARY})
    else()
        set(BUNDLE_ICON_PATH "${MACOS_ICON_PATH}")
    endif()

    set(BUNDLE_RESOURCES $<TARGET_FILE_DIR:${CLIENT_BINARY}>/../Resources)
    add_custom_command(TARGET ${CLIENT_BINARY} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${BUNDLE_RESOURCES}
        COMMAND ${CMAKE_COMMAND} -E copy ${BUNDLE_ICON_PATH} ${BUNDLE_RESOURCES})

    # Configure Info.plist with the bundle's executable name
    set(MACOS_APP_BUNDLE_NAME ${BUNDLE_NAME})
    set(MACOS_APP_EXECUTABLE_NAME ${BUNDLE_NAME})
    set(MACOS_APP_GUI_IDENTIFIER org.openmohaa.${BUNDLE_NAME})
    get_filename_component(MACOS_ICON_FILE ${BUNDLE_ICON_PATH} NAME)
    set(MACOS_APP_ICON_FILE ${MACOS_ICON_FILE})
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

# Replace finish_macos_app with our own function.
# finish_macos_app would generate a plist for "openmohaa" and redirect renderers
# to openmohaa.app/ — all of which we'd then override. Skip it entirely.
list(REMOVE_ITEM POST_CONFIGURE_FUNCTIONS finish_macos_app)
list(APPEND POST_CONFIGURE_FUNCTIONS create_macos_bundle)
