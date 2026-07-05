if(NOT DEFINED GIT_DIR)
    if(DEFINED ENV{GIT_DIR})
        set(GIT_DIR "$ENV{GIT_DIR}")
    else()
        set(GIT_DIR "${CMAKE_SOURCE_DIR}/.git")
    endif()
endif()

if(EXISTS "${GIT_DIR}")
    execute_process(
        COMMAND git describe --tags --always --dirty
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE GIT_DESCRIBE_OUTPUT
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )

    if(GIT_DESCRIBE_OUTPUT)
        string(REGEX MATCH "^v?[0-9]+\\.[0-9]+\\.[0-9]+" TAG_VERSION "${GIT_DESCRIBE_OUTPUT}")
        if(TAG_VERSION)
            set(APP_VERSION "${TAG_VERSION}" CACHE STRING "Application version")
        else()
            set(APP_VERSION "v0.0.0" CACHE STRING "Application version")
        endif()
    else()
        set(APP_VERSION "v0.0.0" CACHE STRING "Application version")
    endif()
else()
    set(APP_VERSION "v0.0.0" CACHE STRING "Application version")
endif()
