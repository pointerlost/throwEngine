#!/bin/bash
set -e

echo "🚀 Building throwEngine..."

# Use relative vcpkg path inside project
VCPKG_TOOLCHAIN_FILE="./vcpkg/scripts/buildsystems/vcpkg.cmake"

if [ ! -f "$VCPKG_TOOLCHAIN_FILE" ]; then
  echo "❌ vcpkg not bootstrapped! Please run ./setup.sh first."
  exit 1
fi

# Detect generator
if command -v ninja >/dev/null 2>&1; then
  GENERATOR="Ninja"
else
  GENERATOR="Unix Makefiles"
fi

echo "⚙️ Using CMake generator: $GENERATOR"

# Configure and build
cmake -B build -G "$GENERATOR" -DCMAKE_TOOLCHAIN_FILE="$VCPKG_TOOLCHAIN_FILE" -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel

echo "✅ Build finished! Run ./build/bin/SampleGame"
