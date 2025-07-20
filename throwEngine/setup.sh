#!/bin/bash
set -e

echo "🚀 Setting up throwEngine..."

# Auto-install vcpkg if missing
if [ ! -d "vcpkg" ]; then
  echo "📦 Installing vcpkg..."
  git clone https://github.com/microsoft/vcpkg.git
  ./vcpkg/bootstrap-vcpkg.sh -disableMetrics
fi

# Configure build
cmake -B build -DCMAKE_TOOLCHAIN_FILE=./vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build --parallel

echo "✅ Setup complete! Run: ./build/bin/SampleGame"