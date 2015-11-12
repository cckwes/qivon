project(qivon_io)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

include_directories(${qivon_io_SOURCE_DIR}/include)
include_directories(${qivon_SOURCE_DIR}/core/include)

set(SOURCES src/pngio.cpp)
set(HEADERS include/pngio.h)

find_package(PNG REQUIRED)

add_library(qivon_io SHARED ${SOURCES} ${HEADERS})
target_link_libraries(qivon_io qivon_core png)