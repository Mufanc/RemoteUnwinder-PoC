cmake_minimum_required(VERSION 3.22.1)
project("unwind-remote")

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(external/abseil-cpp)

add_library(${PROJECT_NAME} SHARED src/main.cpp src/dylib.hpp src/unwinder.cpp src/dlfcn_ex.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE external/unwinding/libunwindstack/include)
target_link_libraries(${PROJECT_NAME} android log absl::strings)
target_compile_definitions(${PROJECT_NAME} PRIVATE NO_DEBUG USE_LOGCAT)
