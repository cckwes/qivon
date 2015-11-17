project(qivon_io)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_definitions(-DQIVON_LIBRARY)

if (unix)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fopenmp")
endif()

include_directories(${qivon_io_SOURCE_DIR}/include)
include_directories(${qivon_SOURCE_DIR}/core/include)

set(SOURCES
        src/pngio.cpp
        src/jpgio.cpp
        )
set(HEADERS
        include/pngio.h
        include/jpgio.h
        )

find_package(PNG REQUIRED)
find_package(JPEG REQUIRED)

if (APPLE)
    include_directories(${PNG_INCLUDE_DIR})
    include_directories(${JPEG_INCLUDE_DIR})
endif()

add_library(qivon_io SHARED ${SOURCES} ${HEADERS})
target_link_libraries(qivon_io qivon_core)
target_link_libraries(qivon_io ${PNG_LIBRARY} ${JPEG_LIBRARY})
