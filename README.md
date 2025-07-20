# throwEngine

A simple and beginner-friendly 3D game engine built with C++, CMake and OpenGL.
This engine currently working with cross-platform.

## About the Engine

throwEngine is a minimal 3D game engine designed
to help beginners(like me -_-) understand the basics of game development and graphics programming.

It's not focused on high performance or advanced features -
instead, it aims to teach you how a simple engine works.

## Features

- 3D rendering with OpenGL (maybe an other time i can add vulkan too.)
- Simple camera and material System
- Basic lighting
- Modular architecture (i'm actually working on it quite a bit.)
- Integration with popular libraries (like GLFW, GLM, stb_image, ImGui, glad and more.)

## Getting Started

### Prerequisites
- C++17 or newer
- CMake >= 3.21
- OpenGL-compatible GPU
- Git
- vcpkg

---

## 📦 Dependencies

Before you build, make sure you have these installed:

- [CMake](https://cmake.org/) ≥ 3.21
- [Git](https://git-scm.com/)
- A C++20 compatible compiler:
  - GCC or Clang on Linux/macOS
  - MSVC on Windows
- [vcpkg](https://github.com/microsoft/vcpkg) (package manager)

---

## 🛠 Quick Setup (Recommended)

Clone and build the engine with one command:

### On Linux/macOS
```bash
git clone https://github.com/pointerlost/throwEngine.git
cd throwEngine
chmod +x setup.sh
./setup.sh
```

### On Windows (PowerShell or CMD)
```bash
git clone https://github.com/pointerlost/throwEngine.git
cd throwEngine
.\setup.bat
```

🚀 Getting Started

You have two options:

    Use local vcpkg in your project folder

    Or set up global vcpkg and configure the environment

Both are supported.

##########################################################

🔧 Project Setup (First Time)
1. Clone the repository

git clone https://github.com/pointerlost/throwEngine.git
cd throwEngine

2. Clone vcpkg (if you don't already have it globally)

git clone https://github.com/microsoft/vcpkg.git
./vcpkg/bootstrap-vcpkg.sh  # Linux/macOS
# or
.\vcpkg\bootstrap-vcpkg.bat # Windows

3. Set VCPKG_ROOT environment variable

# On Linux/macOS (replace with your vcpkg path)
export VCPKG_ROOT=/absolute/path/to/vcpkg

# On Windows PowerShell
$env:VCPKG_ROOT = "C:/path/to/vcpkg"

4. Build the project using CMake presets

cmake --preset=default
cmake --build --preset=default

🧪 Running the Sample Game

After build completes, run the sample executable:

    On Linux/macOS:

./build/bin/SampleGame

    On Windows:

.\build\bin\SampleGame.exe

🛠 Cleaning Build and Dependencies

To remove the build directory and local vcpkg (if installed locally):

    On Linux/macOS:

./clean.sh

    On Windows:

.\clean.bat

