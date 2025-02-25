function(raygun_add_compile_flags target)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
        # -fno-strict-aliasing is needed because of type casts between structs
        # with the same memory layout.
        target_compile_options(${target} PUBLIC -fno-strict-aliasing)
    elseif(MSVC)
        target_compile_options(${target} PUBLIC /Zi /MP)

        target_link_options(${target} PUBLIC
            /LTCG
            /DEBUG
            /NODEFAULTLIB:libcmt.lib
            /ignore:4075 # ignoring /INCREMENTAL due to /LTCG
            /ignore:4098 # defaultlib MSVCRT conflicts with use of other libs
            /ignore:4099 # missing .pdb for .lib
        )
    endif()
endfunction()

function(raygun_enable_warnings target)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
        target_compile_options(${target} PRIVATE -Wall -Wextra)
    elseif(MSVC)
        target_compile_options(${target} PRIVATE /W3)
    endif()
endfunction()
