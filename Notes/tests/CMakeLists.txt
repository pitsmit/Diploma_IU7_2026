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


# =========================
# UNIT TESTS
# =========================
add_executable(unit_tests
    unit/test_mount_point_builder.cpp
    unit/test_mount_registry.cpp
    unit/test_mount_utils.cpp
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
    ${PROJECT_SOURCE_DIR}/api-stubs/api
)

target_link_libraries(unit_tests
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(unit_tests
    PROPERTIES LABELS "unit"
    blkid
)

# =========================
# INTEGRATION TESTS
# =========================
add_executable(integration_tests
    integration/test_device_repository.cpp
    integration/test_get_white_list_command.cpp
    integration/test_add_device_to_white_list_command.cpp
    integration/test_real_mount.cpp
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
    ${PROJECT_SOURCE_DIR}/api-stubs/api
)

target_link_libraries(integration_tests
    GTest::gtest_main
    sqlite3
    udev
    blkid
)

gtest_discover_tests(integration_tests
    PROPERTIES LABELS "integration"
)