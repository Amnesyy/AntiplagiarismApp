set(SOURCES
    src/Comparator.cpp
    src/JsonBuilder.cpp)
    
include(FetchContent)
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(json)
add_library(Comparator ${SOURCES})

target_include_directories(Comparator PUBLIC include)

target_link_libraries(Comparator PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(Comparator PUBLIC Algorythms)
target_link_libraries(Comparator PUBLIC FilePrep)