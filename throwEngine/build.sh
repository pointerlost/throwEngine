#!/bin/bash

if [ -z "$VCPKG_ROOT" ]; then
  echo "VCPKG_ROOT is not set. Trying default at $HOME/vcpkg"
  export VCPKG_ROOT="$HOME/vcpkg"
fi

if [ ! -f "$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" ]; then
  echo "Error: vcpkg toolchain file not found at $VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"
  exit 1
fi

cmake --preset=default
cmake --build build
