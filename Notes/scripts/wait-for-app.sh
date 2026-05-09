#!/bin/bash

set -e

echo "Waiting for app..."

until curl -s http://app:8080/api/v1/whitelist/ > /dev/null
do
  sleep 1
done

echo "App is ready"

./build/tests/e2e_tests