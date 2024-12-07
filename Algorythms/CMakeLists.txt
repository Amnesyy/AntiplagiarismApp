set(SOURCES
    src/KGramGenerator.cpp
    src/WinnowingSelect.cpp
    src/ModuloSelect.cpp)
add_library(Algorythms ${SOURCES})

target_include_directories(Algorythms PUBLIC include)

target_link_libraries(Algorythms PUBLIC FilePrep)