include(FetchContent)
message(STATUS "Fetching Enzyme...")
FetchContent_Declare(
  Enzyme
  GIT_REPOSITORY https://github.com/EnzymeAD/Enzyme
  GIT_TAG v0.0.99
)
FetchContent_MakeAvailable(Enzyme)

