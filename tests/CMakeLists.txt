add_executable(tests)

target_sources(tests
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tests.cpp)

set_target_properties(tests
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED TRUE)

target_link_libraries(tests
    PRIVATE Catch2::Catch2
    PRIVATE Barely::Functional)

if(CLANG_TIDY)
    set_target_properties(tests PROPERTIES
        CXX_CLANG_TIDY "clang-tidy;--use-color")
endif()

if(MSVC)
    target_compile_options(tests
        PRIVATE /W4)
else()
    target_compile_options(tests
        PRIVATE -Wall
        PRIVATE -Werror
        PRIVATE -Wextra)
endif()

add_custom_command(TARGET tests
    POST_BUILD
    COMMAND tests --use-colour yes)
