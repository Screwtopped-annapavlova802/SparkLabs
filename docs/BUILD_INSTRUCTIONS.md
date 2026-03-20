# SparkLabs Engine Build Instructions

This document provides comprehensive build instructions for SparkLabs Engine across all supported platforms.

## Table of Contents

1. [Prerequisites](#prerequisites)
2. [Quick Start](#quick-start)
3. [CMake Build Options](#cmake-build-options)
4. [Windows Build](#windows-build)
5. [macOS Build](#macos-build)
6. [Linux Build](#linux-build)
7. [Optional Components](#optional-components)
8. [Troubleshooting](#troubleshooting)

---

## Prerequisites

### Required Tools

| Tool | Version | Description |
|------|---------|-------------|
| CMake | 3.14+ | Cross-platform build system |
| C++ Compiler | C++17+ | MSVC 19.28+, GCC 9+, or Clang 10+ |
| Git | Any recent | Version control |

### Platform-Specific Requirements

#### Windows
- Visual Studio 2019 (16.8+) or Visual Studio 2022
- Windows 10 SDK (10.0.19041.0+) or later
- Optional: Ninja build system for faster builds

#### macOS
- Xcode 12+ with Command Line Tools
- macOS 10.14 (Mojave) or later
- Optional: Ninja build system

#### Linux
- GCC 9+ or Clang 10+
- libx11-dev, libxrandr-dev, libxinerama-dev, libxcursor-dev, libxi-dev
- libgl1-mesa-dev or libegl1-mesa-dev

### Optional Dependencies

| Component | Package | Description |
|-----------|---------|-------------|
| ONNX Runtime | onnxruntime (v1.10+) | Neural network inference |
| CUDA (NVIDIA) | CUDA Toolkit 11.0+ | GPU acceleration |
| CoreML (Apple) | Built-in | macOS/iOS GPU acceleration |



## Quick Start

### Basic Build

```bash
# Clone the repository
git clone https://github.com/Yuan-ManX/SparkLabs.git
cd SparkLabs

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake ..

# Build
cmake --build . --config Release

# Run tests
ctest --output-on-failure
```

### Build with Tests Enabled

```bash
cmake .. -DSPARK_AI_BUILD_TESTS=ON
cmake --build . --config Release
./bin/SparkLabsTests
```

---

## CMake Build Options

### Core Options

| Option | Type | Default | Description |
|--------|------|---------|-------------|
| `SPARK_AI_BUILD_TESTS` | BOOL | ON | Build unit tests with Google Test |
| `SPARK_AI_BUILD_EDITOR` | BOOL | ON | Build editor tools |
| `SPARK_AI_BUILD_SAMPLES` | BOOL | OFF | Build sample projects |

### AI Options

| Option | Type | Default | Description |
|--------|------|---------|-------------|
| `SPARK_AI_ENABLE_ONNX` | BOOL | ON | Enable ONNX Runtime support |
| `SPARK_AI_ENABLE_GPU` | BOOL | OFF | Enable GPU acceleration |
| `SPARK_AI_ONNX_PROVIDER` | STRING | CPU | ONNX execution provider (CPU/CUDA/CoreML/DirectML) |

### Render Options

| Option | Type | Default | Description |
|--------|------|---------|-------------|
| `SPARK_AI_ENABLE_NN_RENDER` | BOOL | ON | Enable neural rendering effects |
| `SPARK_AI_RENDER_BACKEND` | STRING | Vulkan | Render backend (Vulkan/DirectX12/Metal) |

### Installation Options

| Option | Type | Default | Description |
|--------|------|---------|-------------|
| `CMAKE_INSTALL_PREFIX` | PATH | /usr/local | Installation directory |
| `CMAKE_BUILD_TYPE` | STRING | Release | Build type (Debug/Release/RelWithDebInfo/MinSizeRel) |

### Example: Full GPU Build

```bash
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DSPARK_AI_ENABLE_ONNX=ON \
    -DSPARK_AI_ENABLE_GPU=ON \
    -DSPARK_AI_ONNX_PROVIDER=CUDA \
    -DSPARK_AI_BUILD_TESTS=ON
```

---

## Windows Build

### Visual Studio Generator (Recommended)

1. **Open Command Prompt or PowerShell**

2. **Clone and configure:**
```powershell
git clone https://github.com/Yuan-ManX/SparkLabs.git
cd SparkLabs
mkdir build
cd build

cmake .. -G "Visual Studio 17 2022" -A x64
```

3. **Build:**
```powershell
cmake --build . --config Release
```

4. **Run:**
```powershell
.\bin\Release\SparkLabs.exe
```

### Ninja Generator (Faster Builds)

1. **Install Ninja:**
   - Download from https://github.com/ninja-build/ninja/releases
   - Or via Chocolatey: `choco install ninja`

2. **Configure with Ninja:**
```powershell
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release
```

3. **Build:**
```powershell
cmake --build .
```

### Visual Studio Code Integration

Add to `.vscode/tasks.json`:

```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "Build SparkLabs",
            "type": "shell",
            "command": "cmake --build build --config Release",
            "group": "build"
        },
        {
            "label": "Configure SparkLabs",
            "type": "shell",
            "command": "mkdir -p build && cd build && cmake .. -G \"Visual Studio 17 2022\" -A x64",
            "group": "build"
        }
    ]
}
```

### Build with GPU Support (CUDA)

1. **Install CUDA Toolkit 11.0+**

2. **Configure with CUDA:**
```powershell
cmake .. ^
    -G "Visual Studio 17 2022" -A x64 ^
    -DSPARK_AI_ENABLE_GPU=ON ^
    -DSPARK_AI_ONNX_PROVIDER=CUDA ^
    -DCMAKE_CUDA_COMPILER="C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.8/bin/nvcc.exe"
```

3. **Build:**
```powershell
cmake --build . --config Release
```

### Build with DirectX 12

```powershell
cmake .. ^
    -G "Visual Studio 17 2022" -A x64 ^
    -DSPARK_AI_RENDER_BACKEND=DirectX12
```

---

## macOS Build

### Xcode Generator

1. **Configure:**
```bash
git clone https://github.com/Yuan-ManX/SparkLabs.git
cd SparkLabs
mkdir build && cd build
cmake .. -G Xcode
```

2. **Build:**
```bash
cmake --build . --config Release
```

3. **Run:**
```bash
./bin/SparkLabs
```

### Unix Makefiles

1. **Configure:**
```bash
cmake .. -DCMAKE_BUILD_TYPE=Release
```

2. **Build:**
```bash
cmake --build .
```

3. **Install (optional):**
```bash
sudo cmake --install .
```

### Ninja Generator

1. **Install Ninja:**
```bash
brew install ninja
```

2. **Configure:**
```bash
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release
```

3. **Build:**
```bash
cmake --build .
```

### Build with Metal Rendering

```bash
cmake .. \
    -DSPARK_AI_RENDER_BACKEND=Metal \
    -DCMAKE_BUILD_TYPE=Release
```

### Build with CoreML GPU Acceleration

```bash
cmake .. \
    -DSPARK_AI_ENABLE_GPU=ON \
    -DSPARK_AI_ONNX_PROVIDER=CoreML \
    -DCMAKE_BUILD_TYPE=Release
```

### Universal Binary Build (Intel + Apple Silicon)

```bash
cmake .. \
    -G Xcode \
    -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64" \
    -DCMAKE_BUILD_TYPE=Release
```

### Cross-Compilation for iOS

```bash
cmake .. \
    -G Xcode \
    -DCMAKE_OSX_SYSROOT=iphoneos \
    -DCMAKE_OSX_DEPLOYMENT_TARGET=14.0 \
    -DCMAKE_OSX_ARCHITECTURES=arm64
```

---

## Linux Build

### Ubuntu / Debian

1. **Install dependencies:**
```bash
sudo apt-get update
sudo apt-get install -y \
    build-essential \
    cmake \
    git \
    libx11-dev \
    libxrandr-dev \
    libxinerama-dev \
    libxcursor-dev \
    libxi-dev \
    libgl1-mesa-dev \
    libegl1-mesa-dev
```

2. **Configure and build:**
```bash
git clone https://github.com/Yuan-ManX/SparkLabs.git
cd SparkLabs
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

### Fedora / RHEL / CentOS

1. **Install dependencies:**
```bash
sudo dnf install \
    cmake \
    gcc-c++ \
    git \
    mesa-libGL-devel \
    mesa-libEGL-devel \
    libX11-devel \
    libXrandr-devel \
    libXinerama-devel \
    libXcursor-devel \
    libXi-devel
```

2. **Configure and build:**
```bash
git clone https://github.com/Yuan-ManX/SparkLabs.git
cd SparkLabs
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

### Arch Linux

1. **Install dependencies:**
```bash
sudo pacman -S \
    cmake \
    gcc \
    git \
    mesa \
    libx11 \
    libxrandr \
    libxinerama \
    libxcursor \
    libxi
```

2. **Configure and build:**
```bash
git clone https://github.com/Yuan-ManX/SparkLabs.git
cd SparkLabs
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

### Build with Vulkan

```bash
sudo apt-get install -y libvulkan-dev vulkan-tools
cmake .. \
    -DSPARK_AI_RENDER_BACKEND=Vulkan \
    -DCMAKE_BUILD_TYPE=Release
```

### Build with CUDA (NVIDIA)

1. **Install CUDA Toolkit:**
```bash
# Download from https://developer.nvidia.com/cuda-downloads
sudo apt-get install cuda
```

2. **Configure with CUDA:**
```bash
cmake .. \
    -DSPARK_AI_ENABLE_GPU=ON \
    -DSPARK_AI_ONNX_PROVIDER=CUDA \
    -DCMAKE_BUILD_TYPE=Release
```

### Build with ROCm (AMD)

```bash
cmake .. \
    -DSPARK_AI_ENABLE_GPU=ON \
    -DSPARK_AI_ONNX_PROVIDER=ROCm \
    -DCMAKE_BUILD_TYPE=Release
```

### Ninja Build (Faster)

```bash
sudo apt-get install ninja-build
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

### Install System-Wide

```bash
sudo cmake --install . --prefix /usr/local
```

---

## Optional Components

### ONNX Runtime

ONNX Runtime provides neural network inference capabilities. It's enabled by default.

#### CPU Provider (Default)

```bash
cmake .. -DSPARK_AI_ENABLE_ONNX=ON -DSPARK_AI_ONNX_PROVIDER=CPU
```

#### CUDA Provider (NVIDIA GPU)

```bash
# Requires CUDA Toolkit 11.0+ and cuDNN 8.0+
cmake .. \
    -DSPARK_AI_ENABLE_ONNX=ON \
    -DSPARK_AI_ENABLE_GPU=ON \
    -DSPARK_AI_ONNX_PROVIDER=CUDA \
    -DONNXRUNTIME_DIR=/path/to/onnxruntime
```

#### CoreML Provider (Apple Silicon/Mac)

```bash
cmake .. \
    -DSPARK_AI_ENABLE_ONNX=ON \
    -DSPARK_AI_ENABLE_GPU=ON \
    -DSPARK_AI_ONNX_PROVIDER=CoreML
```

#### DirectML Provider (Windows)

```bash
cmake .. \
    -DSPARK_AI_ENABLE_ONNX=ON \
    -DSPARK_AI_ENABLE_GPU=ON \
    -DSPARK_AI_ONNX_PROVIDER=DirectML
```

### Neural Rendering

Neural rendering is enabled by default and uses the AI neural network system.

#### Disable Neural Rendering

```bash
cmake .. -DSPARK_AI_ENABLE_NN_RENDER=OFF
```

#### Configure Neural Render Settings

```bash
cmake .. \
    -DSPARK_AI_ENABLE_NN_RENDER=ON \
    -DSPARK_AI_NNRender_Precision=FP16
```

### Custom ONNX Model Path

```bash
cmake .. \
    -DSPARK_AI_ENABLE_ONNX=ON \
    -DSPARK_AI_ONNX_MODELS_PATH=/path/to/models
```

---

## Troubleshooting

### Common Issues

#### CMake Cannot Find Compiler

**Error:**
```
CMake Error: No CMAKE_CXX_COMPILER could be found.
```

**Solution:**
```bash
# Ubuntu/Debian
sudo apt-get install build-essential

# macOS
xcode-select --install

# Windows
# Install Visual Studio with C++ tools
```

#### CMake Version Too Old

**Error:**
```
CMake Error: CMake 3.14 or higher is required
```

**Solution:**
```bash
# Ubuntu/Debian
sudo apt-get install cmake3
# Or download from https://cmake.org/download

# macOS
brew install cmake

# Windows
# Download and install from https://cmake.org/download
```

#### ONNX Runtime Not Found

**Error:**
```
Could not find ONNXRuntime
```

**Solution:**
```bash
# Disable ONNX if not needed
cmake .. -DSPARK_AI_ENABLE_ONNX=OFF

# Or install ONNX Runtime
# Ubuntu
sudo apt-get install onnxruntime-dev

# Or set path manually
cmake .. -DONNXRUNTIME_DIR=/path/to/onnxruntime
```

#### GPU Not Detected

**Error:**
```
GPU support requested but no GPU found
```

**Solution:**
```bash
# Ensure NVIDIA drivers installed (Linux)
nvidia-smi

# Ensure CUDA installed
nvcc --version

# Disable GPU if not available
cmake .. -DSPARK_AI_ENABLE_GPU=OFF
```

#### macOS Code Signing Issues

**Error:**
```
dyld: Library not loaded
```

**Solution:**
```bash
# Disable library validation for development
sudo codesign --disable
xattr -cr .
```

#### Linux OpenGL Issues

**Error:**
```
libGL.so not found
```

**Solution:**
```bash
sudo apt-get install libgl1-mesa-dev libgl1-mesa-glx
```

#### Windows SDK Not Found

**Error:**
```
Could not find Windows SDK
```

### Build Performance

#### Use Ninja for Faster Builds

```bash
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build . -j$(nproc)
```

#### Parallel Compilation

```bash
cmake --build . --parallel $(nproc)
```

#### Unity Builds (Experimental)

```bash
cmake .. -DCMAKE_UNITY_BUILD=ON -DCMAKE_UNITY_BUILD_BATCH_SIZE=16
```

### Testing

#### Run Specific Tests

```bash
ctest -R AI --output-on-failure
```

#### Run with Coverage

```bash
cmake .. -DSPARK_AI_BUILD_TESTS=ON -DCOVERAGE=ON
cmake --build .
ctest --output-on-failure
```

#### Debug Build

```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build .
```

### Clean Build

```bash
rm -rf build
mkdir build && cd build
cmake ..
cmake --build .
```

### IDE Integration

#### VS Code

Install extensions:
- C/C++ (Microsoft)
- CMake Tools

Open command palette (Ctrl+Shift+P):
```
CMake: Configure
CMake: Build
```

#### CLion

1. File → Open → Select SparkLabs directory
2. CMake will auto-configure
3. Build → Build Project

#### Visual Studio

1. Open `build/SparkLabs.sln`
2. Select configuration (Debug/Release)
3. Build → Build Solution

#### Xcode

```bash
cmake .. -G Xcode
open SparkLabs.xcodeproj
```

### Release Build

```bash
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=ON
cmake --build . --config Release
```

### Install and Package

```bash
cmake --install . --prefix /path/to/install
cpack --config CPackConfig.cmake
```

---

## Quick Reference

### Minimal Build (Windows)
```powershell
git clone https://github.com/Yuan-ManX/SparkLabs.git
cd SparkLabs
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release
.\bin\Release\SparkLabs.exe
```

### Minimal Build (macOS)
```bash
git clone https://github.com/Yuan-ManX/SparkLabs.git
cd SparkLabs
mkdir build && cd build
cmake .. -G Xcode
cmake --build . --config Release
./bin/SparkLabs
```

### Minimal Build (Linux)
```bash
git clone https://github.com/Yuan-ManX/SparkLabs.git
cd SparkLabs
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
./bin/SparkLabs
```

### Full GPU Build (Linux/CUDA)
```bash
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DSPARK_AI_ENABLE_GPU=ON \
    -DSPARK_AI_ONNX_PROVIDER=CUDA \
    -DSPARK_AI_BUILD_TESTS=ON
cmake --build . -j$(nproc)
```
