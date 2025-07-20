#!/bin/bash
set -e

echo "🚀 Building throwEngine..."

# Use relative vcpkg path inside project
VCPKG_TOOLCHAIN_FILE="./vcpkg/scripts/buildsystems/vcpkg.cmake"

<<<<<<< HEAD
if [ ! -f "$VCPKG_TOOLCHAIN_FILE" ]; then
  echo "❌ vcpkg not bootstrapped! Please run ./setup.sh first."
  exit 1
=======
<<<<<<< HEAD
# Auto-bootstrap vcpkg if needed
if [ ! -f "$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" ]; then
  echo "📦 Bootstrapping vcpkg..."
  git clone https://github.com/microsoft/vcpkg.git "$VCPKG_ROOT"
  "$VCPKG_ROOT/bootstrap-vcpkg.sh" -disableMetrics
=======
if [ ! -f "$VCPKG_TOOLCHAIN_FILE" ]; then
  echo "❌ vcpkg not bootstrapped! Please run ./setup.sh first."
  exit 1
>>>>>>> 316b5f2 (build stuff)
>>>>>>> 72065e1 (build stuff)
fi

# Detect generator
if command -v ninja >/dev/null 2>&1; then
  GENERATOR="Ninja"
else
  GENERATOR="Unix Makefiles"
fi

echo "⚙️ Using CMake generator: $GENERATOR"

<<<<<<< HEAD
# Configure and build
cmake -B build -G "$GENERATOR" -DCMAKE_TOOLCHAIN_FILE="$VCPKG_TOOLCHAIN_FILE" -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel

echo "✅ Build finished! Run ./build/bin/SampleGame"
=======
<<<<<<< HEAD
# Clean old build (optional but safe for CI)
rm -rf build

# Configure
cmake -B build -G "$GENERATOR" \
  -DCMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" \
  -DCMAKE_BUILD_TYPE=Debug

# Build
cmake --build build --parallel

echo "✅ Build finished!"
=======
# Configure and build
cmake -B build -G "$GENERATOR" -DCMAKE_TOOLCHAIN_FILE="$VCPKG_TOOLCHAIN_FILE" -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel

echo "✅ Build finished! Run ./build/bin/SampleGame"

>>>>>>> 316b5f2 (build stuff)
>>>>>>> 72065e1 (build stuff)
