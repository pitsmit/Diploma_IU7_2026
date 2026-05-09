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
    --output-file coverage.tmp.info

lcov --extract coverage.tmp.info \
    '*/src/*' \
    '*/api-stubs/*' \
    --output-file coverage.filtered.info

genhtml coverage.filtered.info \
    --output-directory coverage-report

echo "Coverage report generated in coverage-report/"