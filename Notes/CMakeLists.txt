cmake_minimum_required(VERSION 3.16)
project(MyProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(app
    src/main.cpp
    api-stubs/api/ApiBase.cpp
    api-stubs/api/DefaultApi.cpp
)

option(ENABLE_COVERAGE "Enable coverage" ON)

if(ENABLE_COVERAGE)
    message(STATUS "Coverage ENABLED")
endif()

target_include_directories(app PUBLIC
    ${PROJECT_SOURCE_DIR}/src/commands
    ${PROJECT_SOURCE_DIR}/src/core
    ${PROJECT_SOURCE_DIR}/src/essenses
    ${PROJECT_SOURCE_DIR}/src/facade
    ${PROJECT_SOURCE_DIR}/src/managers
    ${PROJECT_SOURCE_DIR}/src/repositories
    ${PROJECT_SOURCE_DIR}/src/services
    ${PROJECT_SOURCE_DIR}/external/spdlog/include
    ${PROJECT_SOURCE_DIR}/api-stubs/api
)

target_link_libraries(app
    sqlite3
    pistache
    pthread
    udev
    blkid
)

file(COPY ${CMAKE_SOURCE_DIR}/sql
     DESTINATION ${CMAKE_BINARY_DIR}/tests)

enable_testing()
add_subdirectory(tests)

add_compile_options(
    -Wall
    -Wextra
    -Wunused
    -Wshadow
)

if(CMAKE_BUILD_TYPE STREQUAL "Coverage")
    add_compile_options(--coverage -O0 -g)
    add_link_options(--coverage)
endif()