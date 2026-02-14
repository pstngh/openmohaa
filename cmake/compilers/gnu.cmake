# GNU style (GCC/Clang) compiler specific settings

if(NOT CMAKE_C_COMPILER_ID STREQUAL "GNU" AND NOT CMAKE_C_COMPILER_ID MATCHES "^(Apple)?Clang$")
    return()
endif()

enable_language(ASM)

set(ASM_SOURCES
    ${SOURCE_DIR}/asm/ftola.c
    ${SOURCE_DIR}/asm/snapvector.c
)

add_compile_options(-Wall -Wimplicit -Wshadow
    -Wstrict-prototypes -Wformat=2  -Wformat-security
    -Wstrict-aliasing=2 -Wmissing-format-attribute
    -Wdisabled-optimization -Werror-implicit-function-declaration)

add_compile_options(-Wno-format-zero-length -Wno-format-nonliteral)

# There are lots of instances of union based aliasing in the code
# that rely on the compiler not optimising them away, so disable it
add_compile_options(-fno-strict-aliasing)

# This is necessary to hide all symbols unless explicitly exported
# via the Q_EXPORT macro
#add_compile_options(-fvisibility=hidden)

add_compile_options(-Wno-comment)
# Treat no return type as error
add_compile_options(-Werror=return-type)

if(APPLE AND CMAKE_C_COMPILER_ID MATCHES "Clang")
    # Keep legacy renderer sources building on modern macOS Clang toolchains
    # (both AppleClang and Homebrew LLVM Clang) where these diagnostics are
    # noisy and may be promoted to errors by CI environments.
    add_compile_options(
        -Wno-strict-prototypes
        -Wno-deprecated-non-prototype
        -Wno-incompatible-pointer-types-discards-qualifiers
        -Wno-macro-redefined
        -Wno-pointer-sign
        -Wno-unused-function
        -Wno-unused-variable)
endif()

set(ASM_SOURCES)

execute_process(COMMAND ${CMAKE_C_COMPILER} -v
                OUTPUT_VARIABLE COMPILER_OUTPUT
                ERROR_VARIABLE COMPILER_OUTPUT
                OUTPUT_STRIP_TRAILING_WHITESPACE)

list(APPEND CLIENT_LIBRARIES m)
list(APPEND SERVER_LIBRARIES m)

if(COMPILER_OUTPUT MATCHES "musl|freebsd|openbsd|netbsd")
    list(APPEND CLIENT_LIBRARIES execinfo)
    list(APPEND SERVER_LIBRARIES execinfo)
elseif (NOT APPLE)
    # For when using GLIBC versions older than 2.34
    list(APPEND CLIENT_LIBRARIES rt)
    list(APPEND SERVER_LIBRARIES rt)
endif()
