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
  if [[ "$OSTYPE" == "msys" || "$OSTYPE" == "win32" ]]; then
    ./vcpkg/bootstrap-vcpkg.bat
  else
    ./vcpkg/bootstrap-vcpkg.sh
  fi
fi

# Detect generator
if command -v ninja >/dev/null 2>&1; then
  GENERATOR="Ninja"
else
  GENERATOR="Unix Makefiles"
fi
echo "⚙️ Using CMake generator: $GENERATOR"

# Configure build directory
cmake -B build -G "$GENERATOR" -DCMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" -DCMAKE_BUILD_TYPE=Debug

# Build
cmake --build build --parallel

echo "✅ Build finished!"

