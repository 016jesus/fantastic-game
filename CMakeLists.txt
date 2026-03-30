cmake_minimum_required(VERSION 3.16)
project(FantasticGame CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SFML bundled
set(SFML_DIR "${CMAKE_SOURCE_DIR}/SFML/lib/cmake/SFML")
find_package(SFML 2.6 COMPONENTS graphics window audio system REQUIRED)

file(GLOB_RECURSE SOURCES "*.cpp")
list(FILTER SOURCES EXCLUDE REGEX ".*SFML/.*")

add_executable(FantasticGame ${SOURCES})

target_include_directories(FantasticGame PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/SFML/include
)

target_link_libraries(FantasticGame
    sfml-graphics
    sfml-window
    sfml-audio
    sfml-system
)
