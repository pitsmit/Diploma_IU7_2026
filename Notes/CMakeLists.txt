cmake_minimum_required(VERSION 3.16)
project(MyProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(app
    src/main.cpp
    api-stubs/api/ApiBase.cpp
    api-stubs/api/DefaultApi.cpp
)

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
)

enable_testing()
add_subdirectory(tests)