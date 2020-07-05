add_library(BarelyFunctional INTERFACE)
    
target_include_directories(BarelyFunctional
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../include)

add_library(Barely::Functional ALIAS BarelyFunctional)
