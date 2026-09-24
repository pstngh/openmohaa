#
# Persistent chat-ban unit tests
#

add_executable(test_chatban
    ${SOURCE_DIR}/server/tests/test_chatban.c
    ${SOURCE_DIR}/server/sv_chatban_core.c
)

target_include_directories(test_chatban PRIVATE ${SOURCE_DIR})
add_test(NAME test_chatban COMMAND test_chatban)
set_tests_properties(test_chatban PROPERTIES TIMEOUT 15)
