add_library(BarelyFunctional INTERFACE)
    
target_include_directories(BarelyFunctional
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../include)

target_link_libraries(BarelyFunctional)

if(CLANG_TIDY)
    set_target_properties(BarelyFunctional PROPERTIES
        CXX_CLANG_TIDY "clang-tidy;-header-filter=./include")
endif()

if(MSVC)
    target_compile_options(BarelyFunctional
        INTERFACE /W4)
else()
    target_compile_options(BarelyFunctional
        INTERFACE -Wall
        INTERFACE -Werror
        INTERFACE -Wextra)
endif()

add_library(Barely::Functional ALIAS BarelyFunctional)
