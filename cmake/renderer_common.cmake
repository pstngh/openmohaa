include_guard(GLOBAL)

set(RENDERER_COMMON_SOURCES
    ${SOURCE_DIR}/renderercommon/tr_font.c
    ${SOURCE_DIR}/renderercommon/tr_image_bmp.c
    ${SOURCE_DIR}/renderercommon/tr_image_jpg.c
    ${SOURCE_DIR}/renderercommon/tr_image_pcx.c
    ${SOURCE_DIR}/renderercommon/tr_image_png.c
    ${SOURCE_DIR}/renderercommon/tr_image_pvr.c
    ${SOURCE_DIR}/renderercommon/tr_image_tga.c
    ${SOURCE_DIR}/renderercommon/tr_noise.c
    ${SOURCE_DIR}/renderercommon/puff.c
	${SOURCE_DIR}/tiki/tiki_mesh.cpp
)

set(SDL_RENDERER_SOURCES
    ${SOURCE_DIR}/sdl/sdl_gamma.c
    ${SOURCE_DIR}/sdl/sdl_glimp.c
)

# Dear ImGui sources for the in-game overlay menu (F7)
set(IMGUI_DIR ${SOURCE_DIR}/thirdparty/imgui)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_impl_sdl2.cpp
    ${IMGUI_DIR}/imgui_impl_opengl2.cpp
    ${SOURCE_DIR}/client/cl_imgui_menu.cpp
)
list(APPEND SDL_RENDERER_SOURCES ${IMGUI_SOURCES})

set(DYNAMIC_RENDERER_SOURCES
    ${SOURCE_DIR}/renderercommon/tr_subs.c
    ${SOURCE_DIR}/qcommon/q_shared.c
    ${SOURCE_DIR}/qcommon/q_math.c
    ${SOURCE_DIR}/corepp/str.cpp
)

if(USE_FREETYPE)
    list(APPEND RENDERER_DEFINITIONS BUILD_FREETYPE)
endif()

if(USE_RENDERER_DLOPEN)
    list(APPEND RENDERER_DEFINITIONS USE_RENDERER_DLOPEN)
    list(APPEND RENDERER_DEFINITIONS REF_DLL=1)
elseif(BUILD_RENDERER_GL1 AND BUILD_RENDERER_GL2)
    message(FATAL_ERROR "Multiple static renderers enabled; choose one")
elseif(NOT BUILD_RENDERER_GL1 AND NOT BUILD_RENDERER_GL2)
    message(FATAL_ERROR "Zero static renderers enabled; choose one")
endif()

list(APPEND RENDERER_INCLUDE_DIRS ${SOURCE_DIR}/thirdparty/imgui)

# ImGui OpenGL2 backend calls GL functions directly (not via qgl wrappers),
# so we need to link the platform OpenGL library
find_package(OpenGL REQUIRED)
list(APPEND RENDERER_LIBRARIES OpenGL::GL)

list(APPEND RENDERER_LIBRARIES ${COMMON_LIBRARIES})

