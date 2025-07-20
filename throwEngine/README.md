# throwEngine

🎮 A cross-platform C++ game engine built with CMake and vcpkg.

Designed for modularity, flexibility, and simplicity — this engine is ideal for learning or prototyping.

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
cd throwEngine && ./setup.sh
```
On Windows (PowerShell or CMD)
```
git clone https://github.com/pointerlost/throwEngine.git
cd throwEngine
.\setup.bat
```
----------------------------------------------------------
## 🚀 Getting Started

You have two options:
- Use **local vcpkg** in your project folder
- Or set up **global vcpkg** and configure the environment

Both are supported.

---

## 🔧 Project Setup (First Time)

### 1. Clone the repository

```bash
git clone https://github.com/pointerlost/throwEngine.git
cd throwEngine

# 2. Clone vcpkg(if you not already have globally)
git clone https://github.com/microsoft/vcpkg.git
./vcpkg/bootstrap-vcpkg.sh  # Linux/macOS
# or
.\vcpkg\bootstrap-vcpkg.bat # Windows

# 3. set vcpkg_root path
# On Linux/macOS:
export VCPKG_ROOT=/absolute/path/to/vcpkg

# On Windows PowerShell:
$env:VCPKG_ROOT = "C:/path/to/vcpkg"


# 4. Build the project (using presets)
cmake --preset=default
cmake --build --preset=default
