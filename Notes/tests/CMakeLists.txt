include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(unit_tests
    test_mount_point_builder.cpp
    test_mount_registry.cpp
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
gtest_discover_tests(unit_tests)