cmake_minimum_required(VERSION 3.5)

set(ProjectName "glad")
project(${ProjectName})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
set(HEADER_FILES
    "include/glad/glad.h"
    "include/KHR/khrplatform.h"
)
set(SOURCE_FILES
    "src/glad.c"
)
add_library(${ProjectName} ${HEADER_FILES} ${SOURCE_FILES})