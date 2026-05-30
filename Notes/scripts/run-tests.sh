#!/bin/bash

set -e

ctest --test-dir build --output-on-failure


lcov --capture \
    --directory build \
    --base-directory /app \
    --output-file coverage.info

lcov --remove coverage.info \
    '/usr/*' \
    '*/build/*' \
    '*/_deps/*' \
    '*/external/*' \
    '*/tests/*' \
    '*/src/core/IMountSystem.hpp' \
    '*/src/core/IDeviceResolver.hpp' \
    '*/src/core/UdevDeviceResolver.hpp' \
    '*/src/services/Exceptions.hpp' \
    --output-file coverage.tmp.info

lcov --extract coverage.tmp.info \
    '*/src/*' \
    '*/api-stubs/*' \
    --output-file coverage.filtered.info

genhtml coverage.filtered.info \
    --output-directory /app/coverage-report

touch /app/coverage-report/AFTER_GENHTML
sync

echo "=== HOST CHECK MARKER ==="
ls -la /app/coverage-report/AFTER_GENHTML