project(qivon_imgproc)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_definitions(-DQIVON_LIBRARY)

if (unix)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fopenmp")
endif()

include_directories(${qivon_imgproc_SOURCE_DIR}/include)
include_directories(${qivon_SOURCE_DIR}/core/include)

set(SOURCES
        src/color.cpp
        src/filter.cpp
        src/geometry.cpp)
set(HEADERS
        include/color.h
        include/filter.h
        include/geometry.h)

add_library(qivon_imgproc SHARED ${SOURCES} ${HEADERS})
target_link_libraries(qivon_imgproc qivon_core)