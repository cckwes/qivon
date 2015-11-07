project(core)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

include_directories(${core_SOURCE_DIR}/include)

set(SOURCES src/image.cpp)
set(HEADERS include/image.h)

add_library(core SHARED ${SOURCES} ${HEADERS})