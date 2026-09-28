#
# Player movement (bg_pmove) harness: sub-step response tests
#
# pm_harness is the real player movement code (bg_pmove, bg_slidemove,
# bg_misc with q_shared/q_math) driven against PmWorld, a static world of
# axis-aligned boxes and player bodies with the engine's trace semantics.
# Public headers (code/tests/pmove): pm_world.h (PmWorld, PmWorldScope) and
# pm_runner.h (PmPlayer, PmSettings, PmRunUsercmd, PmRunServerFrame, ...).
# Link it and include "pm_runner.h" to run players offline (bot arena).
#

set(PMOVE_HARNESS_DIR ${SOURCE_DIR}/tests/pmove)

add_library(pm_harness STATIC
    ${SOURCE_DIR}/fgame/bg_misc.cpp
    ${SOURCE_DIR}/fgame/bg_pmove.cpp
    ${SOURCE_DIR}/fgame/bg_slidemove.cpp
    ${SOURCE_DIR}/qcommon/q_math.c
    ${SOURCE_DIR}/qcommon/q_shared.c
    ${PMOVE_HARNESS_DIR}/pm_runner.cpp
    ${PMOVE_HARNESS_DIR}/pm_stubs.cpp
    ${PMOVE_HARNESS_DIR}/pm_world.cpp
)
target_include_directories(pm_harness PUBLIC ${PMOVE_HARNESS_DIR})
if(UNIX)
    target_link_libraries(pm_harness PUBLIC m)
endif()

add_executable(test_pmove_substeps ${PMOVE_HARNESS_DIR}/test_pmove_substeps.cpp)
target_link_libraries(test_pmove_substeps PRIVATE pm_harness)

add_test(NAME test_pmove_substeps COMMAND test_pmove_substeps)
set_tests_properties(test_pmove_substeps PROPERTIES TIMEOUT 30)
