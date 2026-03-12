if(NOT BUILD_CLIENT)
    return()
endif()

include(utils/svg_to_icns)

function (create_game_client name output_name target_game icon_svg)
    add_executable(${name} MACOSX_BUNDLE ${CLIENT_BINARY_SOURCES})

    target_include_directories(     ${name} PRIVATE ${CLIENT_INCLUDE_DIRS})
    target_include_directories(     ${name} PRIVATE ${SOURCE_DIR}/client)
    target_compile_definitions(     ${name} PRIVATE ${CLIENT_DEFINITIONS} DEFAULT_TARGET_GAME=${target_game})
    target_compile_options(         ${name} PRIVATE ${CLIENT_COMPILE_OPTIONS})
    target_link_libraries(          ${name} PRIVATE ${COMMON_LIBRARIES} ${CLIENT_LIBRARIES})
    target_link_options(            ${name} PRIVATE ${CLIENT_LINK_OPTIONS})

    set_output_dirs(${name})

    if(NOT USE_RENDERER_DLOPEN)
        target_sources(${name} PRIVATE
            ${RENDERER_GL1_BINARY_SOURCES}
            ${RENDERER_GL2_BINARY_SOURCES})

        target_include_directories( ${name} PRIVATE ${RENDERER_INCLUDE_DIRS})
        target_compile_definitions( ${name} PRIVATE ${RENDERER_DEFINITIONS})
        target_compile_options(     ${name} PRIVATE ${RENDERER_COMPILE_OPTIONS})
        target_link_libraries(      ${name} PRIVATE ${RENDERER_LIBRARIES})
    endif()

    foreach(LIBRARY IN LISTS CLIENT_DEPLOY_LIBRARIES)
        add_custom_command(TARGET ${name} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
                ${LIBRARY}
                $<TARGET_FILE_DIR:${name}>)

        install(FILES ${LIBRARY} DESTINATION
            $<PATH:RELATIVE_PATH,$<TARGET_FILE_DIR:${name}>,${CMAKE_BINARY_DIR}/$<CONFIG>>)
    endforeach()

    set_target_properties(${name} PROPERTIES OUTPUT_NAME "${output_name}${TARGET_BIN_SUFFIX}")
    set_target_properties(${name} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

    INSTALL(TARGETS ${name} DESTINATION ${INSTALL_BINDIR_FULL})

    # Set up macOS .app bundle with per-game icon
    if(APPLE AND BUILD_MACOS_APP)
        set(LAUNCHER_ICNS_PATH "${CMAKE_BINARY_DIR}/${name}.icns")
        svg_to_icns("${icon_svg}" "${LAUNCHER_ICNS_PATH}" TARGET ${name})

        get_filename_component(LAUNCHER_ICNS_FILE "${LAUNCHER_ICNS_PATH}" NAME)

        set(MACOS_APP_BUNDLE_NAME ${output_name})
        set(MACOS_APP_EXECUTABLE_NAME ${output_name}${TARGET_BIN_SUFFIX})
        set(MACOS_APP_GUI_IDENTIFIER org.openmohaa.${name})
        set(MACOS_APP_ICON_FILE ${LAUNCHER_ICNS_FILE})
        set(MACOS_APP_SHORT_VERSION_STRING ${PRODUCT_VERSION})
        set(MACOS_APP_BUNDLE_VERSION ${PRODUCT_VERSION})
        set(MACOS_APP_DEPLOYMENT_TARGET ${CMAKE_OSX_DEPLOYMENT_TARGET})
        set(MACOS_APP_COPYRIGHT ${COPYRIGHT})
        set(MACOS_APP_PLIST_URL_TYPES "")

        configure_file(${CMAKE_SOURCE_DIR}/cmake/Info.plist.in
            ${CMAKE_BINARY_DIR}/${name}-Info.plist @ONLY)

        set_target_properties(${name} PROPERTIES
            MACOSX_BUNDLE_INFO_PLIST ${CMAKE_BINARY_DIR}/${name}-Info.plist)

        set(RESOURCES_DIR $<TARGET_FILE_DIR:${name}>/../Resources)
        add_custom_command(TARGET ${name} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory ${RESOURCES_DIR}
            COMMAND ${CMAKE_COMMAND} -E copy "${LAUNCHER_ICNS_PATH}" ${RESOURCES_DIR})
    endif()
endfunction()

create_game_client(openmohaash openmohaash 1 "${CMAKE_SOURCE_DIR}/misc/openmohaas.svg")
create_game_client(openmohaabt openmohaabt 2 "${CMAKE_SOURCE_DIR}/misc/openmohaab.svg")
