cmake_minimum_required(VERSION 3.26)
project(Calculator)

set(CMAKE_CXX_STANDARD 14)

#add_compile_definitions(DEBUG)

set(SOURCE_FILES
        src/main.cpp
        src/mathExpr.cpp
        src/mathExpr.h
        src/token.h
        src/funcHandler.cpp
        src/funcHandler.h
        src/shuntingYard.cpp
        src/shuntingYard.h
        src/calcException.h
        src/tokenizeException.h
)

add_executable(Calculator ${SOURCE_FILES} icons/app.rc)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")
endif()

target_link_libraries(Calculator m)
