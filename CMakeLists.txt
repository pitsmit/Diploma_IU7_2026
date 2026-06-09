cmake_minimum_required(VERSION 3.16)
project(PrivateDLP)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =========================
# Options
# =========================

option(BUILD_HTTP_SERVER "Build HTTP server" ON)
option(ENABLE_TESTS "Enable test build" ON)
option(ENABLE_TEST_API "Enable test-only HTTP endpoints" OFF)

# =========================
# Sources
# =========================

set(APP_SOURCES
    src/main.cpp
)

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

if(ENABLE_TEST_API)
    target_compile_definitions(app PUBLIC ENABLE_TEST_API)
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
    ixwebsocket
)

if(BUILD_HTTP_SERVER)
    target_link_libraries(app
        pistache
    )
endif()

add_subdirectory(external/IXWebSocket)

# =========================
# Testing
# =========================

if(ENABLE_TESTS)
    file(COPY ${CMAKE_SOURCE_DIR}/sql
        DESTINATION ${CMAKE_BINARY_DIR}/tests)

    file(COPY ${CMAKE_SOURCE_DIR}/config.txt
        DESTINATION ${CMAKE_BINARY_DIR}/tests)
    file(RENAME
        ${CMAKE_BINARY_DIR}/tests/config.txt
        ${CMAKE_BINARY_DIR}/tests/config.base.txt)
        
    file(COPY ${CMAKE_SOURCE_DIR}/config.txt
        DESTINATION ${CMAKE_BINARY_DIR}/tests)

    enable_testing()
    add_subdirectory(tests)
endif()

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