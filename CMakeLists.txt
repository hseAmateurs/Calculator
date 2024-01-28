cmake_minimum_required(VERSION 3.26)
project(Calculator)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
        src/main.cpp
        src/mathExpr.cpp
        src/mathExpr.h
        src/token.h
        src/funcHandler.cpp
        src/funcHandler.h
        src/shuntingYard.cpp
        src/shuntingYard.h
)

add_executable(Calculator ${SOURCE_FILES})

target_link_libraries(Calculator m)
