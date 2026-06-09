include(FetchContent)

if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# =========================
# COVERAGE OPTION HANDLING
# =========================
option(ENABLE_COVERAGE "Enable coverage" ON)

function(enable_coverage target)
    if(ENABLE_COVERAGE)
        target_compile_options(${target} PRIVATE --coverage -O0 -g)
        target_link_options(${target} PRIVATE --coverage)
    endif()
endfunction()

# =========================
# UNIT TESTS
# =========================
add_executable(unit_tests
    unit/test_mount_point_builder.cpp
    unit/test_mount_utils.cpp
    unit/test_config.cpp
)

target_include_directories(unit_tests PUBLIC
    ${PROJECT_SOURCE_DIR}/src/commands
    ${PROJECT_SOURCE_DIR}/src/core
    ${PROJECT_SOURCE_DIR}/src/essenses
    ${PROJECT_SOURCE_DIR}/src/facade
    ${PROJECT_SOURCE_DIR}/src/managers
    ${PROJECT_SOURCE_DIR}/src/repositories
    ${PROJECT_SOURCE_DIR}/src/services
    ${PROJECT_SOURCE_DIR}/external/spdlog/include
)

target_link_libraries(unit_tests
    GTest::gtest_main
)

enable_coverage(unit_tests)

include(GoogleTest)
gtest_discover_tests(unit_tests
    PROPERTIES LABELS "unit"
)

# =========================
# INTEGRATION TESTS
# =========================
add_executable(integration_tests
    integration/test_device_repository.cpp
    integration/test_white_list_command.cpp
    integration/test_real_mount.cpp
    integration/test_delete_device_command.cpp
    integration/test_mount_registry.cpp
)

target_include_directories(integration_tests PUBLIC
    ${PROJECT_SOURCE_DIR}/src/commands
    ${PROJECT_SOURCE_DIR}/src/core
    ${PROJECT_SOURCE_DIR}/src/essenses
    ${PROJECT_SOURCE_DIR}/src/facade
    ${PROJECT_SOURCE_DIR}/src/managers
    ${PROJECT_SOURCE_DIR}/src/repositories
    ${PROJECT_SOURCE_DIR}/src/services
    ${PROJECT_SOURCE_DIR}/external/spdlog/include
)

target_link_libraries(integration_tests
    GTest::gtest_main
    sqlite3
    udev
    blkid
    mount
)

enable_coverage(integration_tests)

gtest_discover_tests(integration_tests
    PROPERTIES LABELS "integration"
)

# =========================
# WARNINGS (GLOBAL SAFE)
# =========================
add_compile_options(
    -Wall
    -Wextra
    -Wunused
    -Wshadow
)


# =========================
# E2E TESTS
# =========================

if(BUILD_E2E_TESTS)

find_package(CURL REQUIRED)

add_executable(e2e_tests
    e2e/test_get_whitelist.cpp
)

target_include_directories(e2e_tests PUBLIC
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

target_link_libraries(e2e_tests
    GTest::gtest_main
    CURL::libcurl
)

gtest_discover_tests(e2e_tests
    PROPERTIES LABELS "e2e"
)

endif()