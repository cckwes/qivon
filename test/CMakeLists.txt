project(test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

include_directories(
        ${qivon_core_SOURCE_DIR}/include
        ${qivon_io_SOURCE_DIR}/include
)

set(SOURCES src/main.cpp)

add_executable(test ${SOURCES})
target_link_libraries(test qivon_core qivon_io)
