project(test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if (unix)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fopenmp")
endif()

include_directories(
        ${qivon_core_SOURCE_DIR}/include
        ${qivon_io_SOURCE_DIR}/include
        ${qivon_imgproc_SOURCE_DIR}/include
        ${test_SOURCE_DIR}/include
)

set(SOURCES
        src/main.cpp
        src/test_color.cpp
        )

set(HEADERS
        include/test_color.h
        )

add_executable(test ${SOURCES} ${HEADERS})
target_link_libraries(test qivon_core qivon_io qivon_imgproc)
