set(HOMEBREW_PREFIX "/opt/homebrew"
    CACHE PATH "Path to Homebrew installation")

set(CMAKE_C_COMPILER "${HOMEBREW_PREFIX}/opt/llvm/bin/clang")

set(CMAKE_PREFIX_PATH
    "${HOMEBREW_PREFIX}"
)

list(TRANSFORM CMAKE_PREFIX_PATH APPEND "/include"
     OUTPUT_VARIABLE CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES)
set(CMAKE_C_STANDARD_INCLUDE_DIRECTORIES "${CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES}")

set(CMAKE_FIND_FRAMEWORK NEVER)
set(CMAKE_FIND_APPBUNDLE NEVER)

set(CMAKE_FIND_USE_CMAKE_SYSTEM_PATH FALSE)
set(CMAKE_FIND_USE_SYSTEM_ENVIRONMENT_PATH FALSE)