project(qivon_core)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_definitions(-DQIVON_LIBRARY)

if (unix)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fopenmp")
endif()

include_directories(${qivon_core_SOURCE_DIR}/include)

set(SOURCES src/image.cpp)
set(HEADERS
        include/image.h include/util.h)

add_library(qivon_core SHARED ${SOURCES} ${HEADERS})