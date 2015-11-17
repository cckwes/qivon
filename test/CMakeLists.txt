project(test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if (unix)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fopenmp")
endif()

include_directories(
        ${qivon_core_SOURCE_DIR}/include
        ${qivon_io_SOURCE_DIR}/include
)

set(SOURCES src/main.cpp)

find_package(PNG REQUIRED)
find_package(JPEG REQUIRED)

if (APPLE)
    include_directories(${PNG_INCLUDE_DIR})
    include_directories(${JPEG_INCLUDE_DIR})
endif()

add_executable(test ${SOURCES})
target_link_libraries(test qivon_core qivon_io)
