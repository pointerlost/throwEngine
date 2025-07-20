#!/bin/bash
set -e

echo "🚀 Setting up throwEngine..."

# Clone vcpkg if missing
if [ ! -d "vcpkg" ]; then
  echo "📦 Cloning vcpkg..."
  git clone https://github.com/microsoft/vcpkg.git
fi

# Bootstrap vcpkg if needed
if [ ! -f "vcpkg/scripts/buildsystems/vcpkg.cmake" ]; then
  echo "⚙️ Bootstrapping vcpkg..."
  ./vcpkg/bootstrap-vcpkg.sh -disableMetrics
fi

echo "✅ Setup complete! Run ./build.sh to build the project."
