if(TARGET igl::core)
    return()
endif()

include(FetchContent)
message(STATUS "Fetching libigl...")
FetchContent_Declare(
    libigl
    GIT_REPOSITORY https://github.com/libigl/libigl.git
    # https://github.com/libigl/libigl/pull/2349
    GIT_TAG fe65ecb907730f96d99a92ebd9c5e852633cc990
)
FetchContent_MakeAvailable(libigl)
