# Add external projects that will be used for the build

if(BUILD_TESTING)
    include(FetchContent)

    FetchContent_Declare(Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.3.1
        GIT_SHALLOW    ON
        CMAKE_ARGS     -DCATCH_INSTALL_DOCS:BOOL=OFF
    )

    # Clear inherited compile options (e.g. -Werror)
    get_property(_COMPILE_OPTIONS DIRECTORY PROPERTY COMPILE_OPTIONS)
    set_property(DIRECTORY PROPERTY COMPILE_OPTIONS "")

    FetchContent_MakeAvailable(Catch2)

    # Restore inherited compile options
    set_property(DIRECTORY PROPERTY COMPILE_OPTIONS ${_COMPILE_OPTIONS})

    list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/contrib)
endif(BUILD_TESTING)
