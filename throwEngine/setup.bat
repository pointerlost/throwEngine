@echo off
echo 🚀 Setting up throwEngine...

:: Auto-install vcpkg if missing
if not exist "vcpkg" (
  echo 📦 Installing vcpkg...
  git clone https://github.com/microsoft/vcpkg.git
  call .\vcpkg\bootstrap-vcpkg.bat -disableMetrics
)

:: Configure build
cmake -B build -DCMAKE_TOOLCHAIN_FILE=.\vcpkg\scripts\buildsystems\vcpkg.cmake
cmake --build build --parallel

echo ✅ Setup complete! Run: .\build\bin\SampleGame.exe