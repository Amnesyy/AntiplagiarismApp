set(SOURCES
    src/Preformatter.cpp
    src/Reader.cpp)

add_library(FilePrep ${SOURCES})

target_include_directories(FilePrep PUBLIC include)