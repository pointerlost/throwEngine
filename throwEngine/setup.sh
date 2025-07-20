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

<<<<<<< HEAD
echo "✅ Setup complete! Run ./build.sh to build the project."
=======
<<<<<<< HEAD
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
=======
echo "✅ Setup complete! Run ./build.sh to build the project."

>>>>>>> 316b5f2 (build stuff)
>>>>>>> 72065e1 (build stuff)
