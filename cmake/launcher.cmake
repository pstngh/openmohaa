if(NOT BUILD_CLIENT)
    return()
endif()

function (create_game_client name output_name target_game)
    add_executable(${name} ${CLIENT_EXECUTABLE_OPTIONS} ${CLIENT_BINARY_SOURCES})

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

    if (MSVC)
        target_link_options(${name} PRIVATE "/MANIFEST:NO")
        INSTALL(FILES $<TARGET_PDB_FILE:${name}> DESTINATION ${INSTALL_BINDIR_FULL} OPTIONAL)
    endif()
endfunction()

create_game_client(openmohaash openmohaash 1)
create_game_client(openmohaabt openmohaabt 2)
