cmake_minimum_required(VERSION 3.5)
project(cc_2026_2027 VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

include_directories(include src/include)

set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})

find_package(OpenMP REQUIRED)

add_executable(omp_merge_array      src/main.cpp)
add_executable(omp_merge_not_array  src/main.cpp)

target_compile_definitions(omp_merge_array     PRIVATE OMP_MERGE_ARRAY)
target_compile_definitions(omp_merge_not_array PRIVATE OMP_MERGE_NOT_ARRAY)

target_link_libraries(omp_merge_array     PRIVATE OpenMP::OpenMP_CXX)
target_link_libraries(omp_merge_not_array PRIVATE OpenMP::OpenMP_CXX)


