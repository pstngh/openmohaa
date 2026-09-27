#
# Movement telemetry logger: column contract tests
#

# Engine-independent: checks code/fgame/movement_telemetry_schema.h against the
# golden headers and HB_DIAG_FIELDS.
add_executable(test_telemetry_headers
    ${SOURCE_DIR}/tests/humanbot/test_telemetry_headers.cpp
)
target_include_directories(test_telemetry_headers PRIVATE ${SOURCE_DIR}/fgame)

add_test(
    NAME test_telemetry_headers
    COMMAND test_telemetry_headers ${SOURCE_DIR}/tests/humanbot/golden
)
set_tests_properties(test_telemetry_headers PROPERTIES TIMEOUT 15)
