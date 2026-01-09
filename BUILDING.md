## Windows

#### 1. Install Git 
- If you do not already have Git installed, install it from [here](https://git-scm.com/install/windows).

#### 2. Clone Repository and Update Submodules
```bash
git clone https://github.com/BlinkDynamo/blinks-thinks.git blinks-thinks
cd blinks-thinks/
git submodule update --init --recursive
```

#### 3. Enable Developer Mode in Windows
- Settings -> Update & Security -> For developers -> Developer Mode
- This allows the next step to be applied correctly

#### 4. Enable Git Symlinks
```bash
git config core.symlinks true
git checkout HEAD -- include/raylib.h include/raymath.h include/rlgl.h
```

#### 5. Install Python
- Download Python from https://www.python.org/downloads/
- During installation, check "Add Python to PATH"

#### 6. Install w64devkit
- Download `w64devkit-x.xx.x.zip` from https://github.com/skeeto/w64devkit/releases
- Extract to `c:\w64devkit`
- Always run `w64devkit.exe` as administrator when developing

#### 7. Install Emscripten SDK
Open w64devkit as administrator, then enter these commands:
```bash
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk.bat install latest
./emsdk.bat activate latest --system
./emsdk_env.bat
```

#### 8. Build The Game

Open w64devkit as administrator and navigate to the project root (blinks-thinks) if you are not already there.

Native build:
```bash
make windows  # Build both debug and release.
```

This creates:
- `build/windows/debug/blinks_thinks.exe`
- `build/windows/release/blinks_thinks.exe`

If this is the first build, raylib will be compiled from source and added to `lib/windows/`.

Web build:
```bash
make web    # Build both debug and release.
make serve  # Serve the HTML locally on port 8080.
```
If this is the first web build, raylib will be compile the web library and add it to `lib/web/`.

#### 9. Play The Game

Run the executable from your terminal or via the File Explorer by double clicking.
```
./build/windows/release/blinks_thinks.exe
```

## Linux

#### 1. Install Build Dependencies:
Fedora:
```bash
sudo dnf install \
git \
clang \
python \
alsa-lib-devel \
mesa-libGL-devel \
libX11-devel \
libXrandr-devel \
libXi-devel \
libXcursor-devel \
libXinerama-devel \
wayland-devel \
libxkbcommon-devel
```

#### 2. Clone Repository and Update Submodules
```bash
git clone https://github.com/BlinkDynamo/blinks-thinks.git blinks-thinks
cd blinks-thinks/
git submodule update --init --recursive
```

#### 3. Install Emscripten SDK
Install and activate Emscripten:
```bash
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
./emsdk activate latest --system
./emsdk_env
```

#### 4. Build The Game

Build the project:

Native build:
```bash
make linux  # Build both debug and release.
```

This creates:
- `build/linux/debug/blinks_thinks`
- `build/linux/release/blinks_thinks`

If this is the first build, raylib will be compiled from source and added to `lib/linux/`.

Web build:
```bash
make web    # Build both debug and release.
make serve  # Serve the HTML locally on port 8080.
```
If this is the first web build, raylib will be compile the web library and add it to `lib/web/`.

#### 5. Play The Game

Run the executable from your terminal or via your file manager of choice by double clicking.
```bash
./build/linux/release/blinks_thinks
```
