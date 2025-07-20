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

cmake --preset=default
cmake --build build

