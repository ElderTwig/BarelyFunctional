add_library(BarelyFunctional_lib INTERFACE)
    
target_include_directories(BarelyFunctional_lib
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../include)

target_link_libraries(BarelyFunctional_lib)

if(CLANG_TIDY)
    set_target_properties(BarelyFunctional_lib PROPERTIES
        CXX_CLANG_TIDY "clang-tidy;-header-filter=./include")
endif()

if(MSVC)
    target_compile_options(BarelyFunctional_lib
        INTERFACE /W4)
else()
    target_compile_options(BarelyFunctional_lib
        INTERFACE -Wall
        INTERFACE -Werror
        INTERFACE -Wextra)
endif()

add_library(BarelyFunctional::lib ALIAS BarelyFunctional_lib)
