# Building Blink's Thinks

This document provides instructions for building the project on Windows, Linux, and for the web.

## Getting Started

First, clone the repository and initialize the raylib submodule:

```bash
git clone https://github.com/BlinkDynamo/blinks-thinks.git blinks-thinks
cd blinks-thinks/
git submodule update --init --recursive
```

## Platform-Specific Setup

### Windows

#### Prerequisites

**w64devkit**
- Download `w64devkit-x.xx.x.zip` from https://github.com/skeeto/w64devkit
- Extract to `c:\w64devkit`

#### Building

1. Run `w64devkit.exe` from `c:\w64devkit` to open the development console.
2. Navigate to your project directory:
   ```bash
   cd /path/to/blinks-thinks
   ```
3. Build the project:
   ```bash
   make native    # Build native debug and release (downloads raylib if needed)
   ```

This creates:
- `build/windows/debug/blinks_thinks.exe`
- `build/windows/release/blinks_thinks.exe`

On first build, raylib will be downloaded automatically (version matches the submodule).

#### Running

From the w64devkit console:
```
./build/windows/release/blinks_thinks.exe
```

Or double-click the executable in File Explorer.

### Linux

#### Prerequisites

1. **Compiler**: clang with C++23 support
2. **Build dependencies** for raylib:
   - Debian/Ubuntu: `sudo apt install build-essential libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev libwayland-dev libxkbcommon-dev`
   - Fedora: `sudo dnf install alsa-lib-devel mesa-libGL-devel libX11-devel libXrandr-devel libXi-devel libXcursor-devel libXinerama-devel wayland-devel libxkbcommon-devel`
   - Arch: `sudo pacman -S alsa-lib mesa libx11 libxrandr libxi libxcursor libxinerama wayland libxkbcommon`

#### Building

```bash
make native    # Build native debug and release (builds raylib if needed)
```

This creates:
- `build/linux/debug/blinks_thinks`
- `build/linux/release/blinks_thinks`

On first build, raylib will be compiled from the submodule with Wayland and X11 support.

#### Running

```bash
./build/linux/release/blinks_thinks
```

### Web

Web builds work from any platform using Emscripten.

#### Prerequisites

1. **Emscripten SDK**
   ```bash
   git clone https://github.com/emscripten-core/emsdk.git
   cd emsdk
   ./emsdk install latest
   ./emsdk activate latest
   source ./emsdk_env.sh
   ```

2. **Python 3**: For running the local web server

#### Building

1. Activate Emscripten environment:
   ```bash
   source /path/to/emsdk/emsdk_env.sh
   ```

2. Build the project:
   ```bash
   make web    # Build web debug and release (builds raylib if needed)
   ```

This creates:
- `build/web/debug/` - Debug build (unoptimized, larger file size)
- `build/web/release/` - Release build (optimized with -O3)

Each contains:
- `index.html`
- `index.js`
- `index.wasm`
- `index.data` (preloaded assets)

On first build, raylib will be compiled from the submodule.

#### Running

Start a local web server:
```bash
make serve
```

Then open http://localhost:8080 in your browser.

## Makefile Targets

- `make` or `make all`: Build both native and web (debug and release for each)
- `make native`: Build native debug and release (Windows/Linux detected automatically)
- `make web`: Build web debug and release (requires Emscripten)
- `make clean`: Remove all build artifacts and libraries
- `make serve`: Build web release and start local server
- `make info`: Display platform and compiler information

## Build Output Structure

```
build/
├── windows/
│   ├── debug/
│   │   └── blinks_thinks.exe
│   └── release/
│       └── blinks_thinks.exe
├── linux/
│   ├── debug/
│   │   └── blinks_thinks
│   └── release/
│       └── blinks_thinks
└── web/
    ├── debug/
    │   ├── index.html
    │   ├── index.js
    │   ├── index.wasm
    │   └── index.data
    └── release/
        ├── index.html
        ├── index.js
        ├── index.wasm
        └── index.data

lib/
├── windows/
│   └── libraylib.a (downloaded from GitHub release)
├── linux/
│   └── libraylib.a (built from external/raylib submodule)
└── web/
    └── libraylib_web.a (built from external/raylib submodule)

include/
├── raylib.h -> ../external/raylib/src/raylib.h (symlink)
├── raymath.h -> ../external/raylib/src/raymath.h (symlink)
├── rlgl.h -> ../external/raylib/src/rlgl.h (symlink)
└── *.hpp (game headers)

Raylib version: Determined by external/raylib submodule (currently 5.5)
```

## Troubleshooting

### Windows

**Problem**: `raylib.h: No such file or directory`
- Ensure git submodule is initialized: `git submodule update --init --recursive`
- Headers come from `external/raylib/src/`

**Problem**: `cannot find -lraylib`
- Run `make native` to download raylib library
- Ensure `lib/windows/libraylib.a` exists after download
- Check internet connection if download fails

**Problem**: `make: command not found`
- Run commands from the w64devkit console, not cmd.exe or PowerShell

### Linux

**Problem**: `raylib.h: No such file or directory`
- Ensure git submodule is initialized: `git submodule update --init --recursive`
- Headers come from `external/raylib/src/`

**Problem**: `cannot find -lraylib`
- Run `make native` to build raylib library
- Install build dependencies (see Prerequisites)
- Ensure `lib/linux/libraylib.a` exists after building

**Problem**: Build errors about missing GLFW or OpenGL
- Install required development libraries (see Prerequisites)

### Web

**Problem**: `emcc: command not found`
- Activate the Emscripten SDK environment:
  ```bash
  source /path/to/emsdk/emsdk_env.sh
  ```

**Problem**: Assets not loading
- Ensure the `res/` directory exists and contains your assets
- The Makefile automatically preloads all files in `res/`

**Problem**: Web build runs but displays incorrectly
- Check browser console for WebGL errors
- Ensure your browser supports WebAssembly and WebGL 2.0
