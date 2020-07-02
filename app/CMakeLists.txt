add_executable(BarelyFunctional main.cpp)

target_link_libraries(BarelyFunctional
    PRIVATE BarelyFunctional::lib)

set_target_properties(BarelyFunctional
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED TRUE)

if(CMAKE_BUILD_TYPE STREQUAL Release)
    set_target_properties(BarelyFunctional
        PROPERTIES
            INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

if(CLANG_TIDY)
    set_target_properties(BarelyFunctional PROPERTIES
        CXX_CLANG_TIDY "clang-tidy;-header-filter=./include")
endif()

if(MSVC)
    target_compile_options(BarelyFunctional
        PRIVATE /W4)
else()
    target_compile_options(BarelyFunctional
        PRIVATE -Wall
        PRIVATE -Werror
        PRIVATE -Wextra)
endif()

if(WIN32 AND CMAKE_BUILD_TYPE STREQUAL Release)
  target_link_options(BarelyFunctional
        PRIVATE /SUBSYSTEM:WINDOWS
        PRIVATE /ENTRY:mainCRTStartup)
endif()
