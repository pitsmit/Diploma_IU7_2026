cmake_minimum_required(VERSION 3.16)
project(MyProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =========================
# Options
# =========================

option(ENABLE_COVERAGE "Enable coverage" ON)
option(BUILD_HTTP_SERVER "Build HTTP server" ON)
option(BUILD_E2E_TESTS "Build e2e tests" OFF)

# =========================
# Sources
# =========================

set(APP_SOURCES
    src/main.cpp
)

# HTTP sources
if(BUILD_HTTP_SERVER)
    list(APPEND APP_SOURCES
        api-stubs/api/ApiBase.cpp
        api-stubs/api/DefaultApi.cpp
    )
endif()

# =========================
# Executable
# =========================

add_executable(app ${APP_SOURCES})

# =========================
# Coverage
# =========================

if(ENABLE_COVERAGE)
    message(STATUS "Coverage ENABLED")
endif()

# =========================
# Includes
# =========================

target_include_directories(app PUBLIC
    ${PROJECT_SOURCE_DIR}/src/commands
    ${PROJECT_SOURCE_DIR}/src/core
    ${PROJECT_SOURCE_DIR}/src/essenses
    ${PROJECT_SOURCE_DIR}/src/facade
    ${PROJECT_SOURCE_DIR}/src/managers
    ${PROJECT_SOURCE_DIR}/src/repositories
    ${PROJECT_SOURCE_DIR}/src/services
    ${PROJECT_SOURCE_DIR}/external/spdlog/include
)

# HTTP includes
if(BUILD_HTTP_SERVER)
    target_include_directories(app PUBLIC
        ${PROJECT_SOURCE_DIR}/api-stubs/api
    )
endif()

# =========================
# Compile definitions
# =========================

if(BUILD_HTTP_SERVER)
    target_compile_definitions(app PUBLIC BUILD_HTTP_SERVER)
endif()

# =========================
# Link libraries
# =========================

target_link_libraries(app
    sqlite3
    pthread
    udev
    blkid
    mount
)

# HTTP link
if(BUILD_HTTP_SERVER)
    target_link_libraries(app
        pistache
    )
endif()

# =========================
# Testing
# =========================

file(COPY ${CMAKE_SOURCE_DIR}/sql
     DESTINATION ${CMAKE_BINARY_DIR}/tests)

file(COPY ${CMAKE_SOURCE_DIR}/config.txt
     DESTINATION ${CMAKE_BINARY_DIR}/tests)

enable_testing()
add_subdirectory(tests)

# =========================
# Warnings
# =========================

add_compile_options(
    -Wall
    -Wextra
    -Wunused
    -Wshadow
)

# =========================
# Coverage flags
# =========================

if(CMAKE_BUILD_TYPE STREQUAL "Coverage")
    add_compile_options(--coverage -O0 -g)
    add_compile_options(-fno-inline)
    add_compile_options(-fno-default-inline)
    add_link_options(--coverage)
endif()