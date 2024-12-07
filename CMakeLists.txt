cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(AntiplagiarismApp)

add_subdirectory(FilePrep)
add_subdirectory(Algorythms)
add_subdirectory(Comparator)

add_executable(myExe main.cpp)

target_include_directories(
    myExe PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/FilePrep/include"
    "${PROJECT_SOURCE_DIR}/Algorythms/include"
)

target_link_libraries(myExe PUBLIC FilePrep Algorythms Comparator)