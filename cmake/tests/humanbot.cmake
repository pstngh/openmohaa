#
# Human-imitation bot brain: unit tests, replay harness and arena
#

file(GLOB HB_CORE_SOURCES ${SOURCE_DIR}/humanbot/*.cpp)

add_library(hb_core STATIC ${HB_CORE_SOURCES})
target_include_directories(hb_core PUBLIC ${SOURCE_DIR}/humanbot)

function(hb_add_test name)
    add_executable(${name} ${ARGN})
    target_include_directories(${name} PRIVATE ${SOURCE_DIR}/tests/humanbot)
    target_link_libraries(${name} PRIVATE hb_core)
    add_test(NAME ${name} COMMAND ${name})
    set_tests_properties(${name} PROPERTIES TIMEOUT 120)
endfunction()

hb_add_test(test_hb_core ${SOURCE_DIR}/tests/humanbot/test_hb_core.cpp)
hb_add_test(test_hb_modules ${SOURCE_DIR}/tests/humanbot/test_hb_modules.cpp)
hb_add_test(test_hb_belief ${SOURCE_DIR}/tests/humanbot/test_hb_belief.cpp)

# Replay harness (not a CTest: it needs the git-ignored exports of the private recordings)
add_executable(hb_replay ${SOURCE_DIR}/tests/humanbot/hb_replay.cpp)
target_link_libraries(hb_replay PRIVATE hb_core)

# Closed-loop arena: bots on the real player movement (pm_harness) in a box world
add_executable(hb_arena ${SOURCE_DIR}/tests/humanbot/hb_arena.cpp)
target_link_libraries(hb_arena PRIVATE hb_core pm_harness)
add_test(NAME test_hb_arena COMMAND hb_arena --test --seconds 120)
set_tests_properties(test_hb_arena PROPERTIES TIMEOUT 600)
