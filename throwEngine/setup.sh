#!/bin/bash
set -e

echo "🚀 Setting up throwEngine..."

# Clone vcpkg if missing
if [ ! -d "vcpkg" ]; then
  echo "📦 Installing vcpkg..."
  git clone https://github.com/microsoft/vcpkg.git
  ./vcpkg/bootstrap-vcpkg.sh -disableMetrics
fi

# Detect generator
if command -v ninja >/dev/null 2>&1; then
  GENERATOR="Ninja"
else
  GENERATOR="Unix Makefiles"
fi
echo "⚙️ Using CMake generator: $GENERATOR"

# Configure
cmake -B build -G "$GENERATOR" \
  -DCMAKE_TOOLCHAIN_FILE=./vcpkg/scripts/buildsystems/vcpkg.cmake \
  -DCMAKE_BUILD_TYPE=Debug

# Build
cmake --build build --parallel

echo "✅ Setup complete! Run: ./build/bin/SampleGame"
