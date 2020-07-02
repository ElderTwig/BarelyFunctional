file(GLOB SRC
    "*.cpp")

add_library(BarelyFunctional_lib STATIC)
    
target_sources(BarelyFunctional_lib 
    PUBLIC ${SRC})

target_include_directories(BarelyFunctional_lib
    PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(BarelyFunctional_lib)

set_target_properties(BarelyFunctional_lib
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED TRUE)

if(CMAKE_BUILD_TYPE STREQUAL Release)
    set_target_properties(BarelyFunctional_lib
        PROPERTIES
            INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

if(CLANG_TIDY)
    set_target_properties(BarelyFunctional_lib PROPERTIES
        CXX_CLANG_TIDY "clang-tidy;-header-filter=./include")
endif()

if(MSVC)
    target_compile_options(BarelyFunctional_lib
        PRIVATE /W4)
else()
    target_compile_options(BarelyFunctional_lib
        PRIVATE -Wall
        PRIVATE -Werror
        PRIVATE -Wextra)
endif()

add_library(BarelyFunctional::lib ALIAS BarelyFunctional_lib)
