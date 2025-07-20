#!/bin/bash
set -e

echo "🚀 Building throwEngine..."

# Set fallback VCPKG_ROOT
if [ -z "$VCPKG_ROOT" ]; then
  export VCPKG_ROOT="$HOME/vcpkg"
  echo "🔍 VCPKG_ROOT not set. Using: $VCPKG_ROOT"
fi

# Auto-bootstrap vcpkg if needed
if [ ! -f "$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" ]; then
  echo "📦 Bootstrapping vcpkg..."
  git clone https://github.com/microsoft/vcpkg.git "$VCPKG_ROOT"
  "$VCPKG_ROOT/bootstrap-vcpkg.sh" -disableMetrics
fi

# Detect generator
if command -v ninja >/dev/null 2>&1; then
  GENERATOR="Ninja"
else
  GENERATOR="Unix Makefiles"
fi
echo "⚙️ Using CMake generator: $GENERATOR"

# Clean old build (optional but safe for CI)
rm -rf build

# Configure
cmake -B build -G "$GENERATOR" \
  -DCMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" \
  -DCMAKE_BUILD_TYPE=Debug

# Build
cmake --build build --parallel

echo "✅ Build finished!"
