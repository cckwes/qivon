project(qivon_io)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if (unix)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fopenmp")
endif()

include_directories(${qivon_io_SOURCE_DIR}/include)
include_directories(${qivon_SOURCE_DIR}/core/include)

set(SOURCES src/pngio.cpp)
set(HEADERS include/pngio.h)

find_package(PNG REQUIRED)
if (APPLE)
    include_directories(${PNG_INCLUDE_DIR})
endif()

add_library(qivon_io SHARED ${SOURCES} ${HEADERS})
target_link_libraries(qivon_io qivon_core)
target_link_libraries(qivon_io ${PNG_LIBRARY})
