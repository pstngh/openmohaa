# Added in OPM
# Single GUI launcher replacing the three launch_openmohaa_* executables
# Windows only

if(WIN32 AND BUILD_CLIENT)
    set(LAUNCHER_SOURCES
        ${SOURCE_DIR}/Launcher/launcher_settings.cpp
        ${SOURCE_DIR}/Launcher/launcher_launch.cpp
        ${SOURCE_DIR}/Launcher/launcher_win32.cpp
        ${SOURCE_DIR}/Launcher/launcher_resource.rc
    )

    add_executable(launcher ${LAUNCHER_SOURCES})
    target_compile_features(launcher PRIVATE cxx_std_17)
    set_target_properties(launcher PROPERTIES OUTPUT_NAME "launcher${TARGET_BIN_SUFFIX}")
    set_target_properties(launcher PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
    set_target_properties(launcher PROPERTIES WIN32_EXECUTABLE TRUE)
    target_link_libraries(launcher PRIVATE comctl32)

    INSTALL(TARGETS launcher DESTINATION ${INSTALL_BINDIR_FULL})
endif()
