#
# Human-imitation bot brain: unit tests, replay harness and arena
#

file(GLOB HB_CORE_SOURCES ${SOURCE_DIR}/humanbot/*.cpp)

if(HB_CORE_SOURCES)
    add_library(hb_core STATIC ${HB_CORE_SOURCES})
    target_include_directories(hb_core PUBLIC ${SOURCE_DIR}/humanbot)
endif()
