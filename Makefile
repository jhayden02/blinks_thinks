.PHONY: all native web clean serve info

# ------------------------
# Platform Detection
# ------------------------
UNAME_S := $(shell uname -s 2>/dev/null || echo Windows)

# Determine native platform (for 'make native').
ifeq ($(findstring MINGW,$(UNAME_S)),MINGW)
    NATIVE_PLATFORM := windows
else ifeq ($(findstring MSYS,$(UNAME_S)),MSYS)
    NATIVE_PLATFORM := windows
else ifeq ($(findstring Windows,$(UNAME_S)),Windows)
    NATIVE_PLATFORM := windows
else ifeq ($(UNAME_S),Linux)
    NATIVE_PLATFORM := linux
else
    NATIVE_PLATFORM := unknown
endif

# PLATFORM is set internally by build targets, not by users.
PLATFORM ?= $(NATIVE_PLATFORM)

# ------------------------
# Compiler Configuration
# ------------------------
STD       := -std=c++23
WARNINGS  := -Wall -Wextra -Werror -Wshadow -Wnon-virtual-dtor -Wold-style-cast
INCLUDES  := -I. -Iinclude

# Platform-specific compilers.
ifeq ($(PLATFORM),windows)
    CXX := g++
    CC  := gcc
else ifeq ($(PLATFORM),linux)
    CXX := clang++
    CC  := clang
else ifeq ($(PLATFORM),web)
    CXX := em++
    CC  := emcc
endif

# ------------------------
# Directory Configuration
# ------------------------
D_SRC := src

# Platform-specific build directories.
ifeq ($(PLATFORM),windows)
    D_OBJ_DEBUG   := build/windows/debug
    D_OBJ_RELEASE := build/windows/release
else ifeq ($(PLATFORM),linux)
    D_OBJ_DEBUG   := build/linux/debug
    D_OBJ_RELEASE := build/linux/release
endif

# Web directories (always defined).
D_OBJ_WEB_DEBUG   := build/web/debug
D_OBJ_WEB_RELEASE := build/web/release

# ------------------------
# Build Flags
# ------------------------
CXXFLAGS_DEBUG   := $(STD) $(WARNINGS) $(INCLUDES)
CXXFLAGS_RELEASE := $(STD) $(WARNINGS) $(INCLUDES) -DNDEBUG -O3

# Web flags (always defined).
CXXFLAGS_WEB_DEBUG   := $(STD) $(WARNINGS) $(INCLUDES) -DPLATFORM_WEB
CXXFLAGS_WEB_RELEASE := $(STD) $(WARNINGS) $(INCLUDES) -DPLATFORM_WEB -DNDEBUG -O3

# ------------------------
# Link Flags
# ------------------------
ifeq ($(PLATFORM),windows)
    LINK_FLAGS := -Llib/windows -lraylib -lgdi32 -lwinmm
    EXECUTABLE_EXT := .exe
else ifeq ($(PLATFORM),linux)
    LINK_FLAGS := -Llib/linux -lraylib -lm -ldl -lpthread -lGL
    # Add Wayland libraries for native Wayland support
    # LINK_FLAGS += -lwayland-client -lwayland-cursor -lwayland-egl -lxkbcommon
    EXECUTABLE_EXT :=
endif

# ------------------------
# Raylib Configuration
# ------------------------
RL_SRC := external/raylib/src
RAYLIB_VERSION := $(shell git -C external/raylib describe --tags --abbrev=0 2>/dev/null || echo "5.0")

ifeq ($(PLATFORM),windows)
    RL_LIB := lib/windows/libraylib.a
else ifeq ($(PLATFORM),linux)
    RL_LIB := lib/linux/libraylib.a
else ifeq ($(PLATFORM),web)
    RL_LIB := lib/web/libraylib_web.a
endif

# Web library (always defined).
RL_LIB_WEB := lib/web/libraylib_web.a

# Web link configuration (always defined).
WEB_PRELOAD_ASSETS := $(shell find res -type f 2>/dev/null | xargs -I{} echo --preload-file {})
WEB_LINK_FLAGS := lib/web/libraylib_web.a \
                  -s USE_GLFW=3 -s ASYNCIFY -s ALLOW_MEMORY_GROWTH=1 \
                  -s FORCE_FILESYSTEM=1 $(WEB_PRELOAD_ASSETS) \
                  --shell-file web/shell.html

# ------------------------
# Source Files
# ------------------------
SOURCES := $(wildcard $(D_SRC)/*.cpp)

# ------------------------
# Build Targets
# ------------------------
all: native web

.NOTPARALLEL: all native web

native: $(RL_LIB) $(D_OBJ_DEBUG)/blinks_thinks$(EXECUTABLE_EXT) $(D_OBJ_RELEASE)/blinks_thinks$(EXECUTABLE_EXT)

web: $(RL_LIB_WEB) $(D_OBJ_WEB_DEBUG)/index.html $(D_OBJ_WEB_RELEASE)/index.html

# ------------------------
# Native Build Rules (Windows/Linux)
# ------------------------
ifneq ($(PLATFORM),web)

$(D_OBJ_DEBUG)/%.o: $(D_SRC)/%.cpp | $(D_OBJ_DEBUG)
	$(CXX) $(CXXFLAGS_DEBUG) -c $< -o $@

$(D_OBJ_DEBUG)/blinks_thinks$(EXECUTABLE_EXT): $(patsubst $(D_SRC)/%.cpp,$(D_OBJ_DEBUG)/%.o,$(SOURCES)) | $(D_OBJ_DEBUG)
	$(CXX) $^ $(LINK_FLAGS) -o $@

$(D_OBJ_RELEASE)/%.o: $(D_SRC)/%.cpp | $(D_OBJ_RELEASE)
	$(CXX) $(CXXFLAGS_RELEASE) -c $< -o $@

$(D_OBJ_RELEASE)/blinks_thinks$(EXECUTABLE_EXT): $(patsubst $(D_SRC)/%.cpp,$(D_OBJ_RELEASE)/%.o,$(SOURCES)) | $(D_OBJ_RELEASE)
	$(CXX) $^ $(LINK_FLAGS) -o $@

endif

# ------------------------
# Web Build Rules (always available)
# ------------------------
$(D_OBJ_WEB_DEBUG)/%.o: $(D_SRC)/%.cpp | $(D_OBJ_WEB_DEBUG)
	em++ $(CXXFLAGS_WEB_DEBUG) -c $< -o $@

$(D_OBJ_WEB_DEBUG)/index.html: $(patsubst $(D_SRC)/%.cpp,$(D_OBJ_WEB_DEBUG)/%.o,$(SOURCES)) $(RL_LIB_WEB) | $(D_OBJ_WEB_DEBUG)
	em++ $(patsubst $(D_SRC)/%.cpp,$(D_OBJ_WEB_DEBUG)/%.o,$(SOURCES)) $(WEB_LINK_FLAGS) -o $@

$(D_OBJ_WEB_RELEASE)/%.o: $(D_SRC)/%.cpp | $(D_OBJ_WEB_RELEASE)
	em++ $(CXXFLAGS_WEB_RELEASE) -c $< -o $@

$(D_OBJ_WEB_RELEASE)/index.html: $(patsubst $(D_SRC)/%.cpp,$(D_OBJ_WEB_RELEASE)/%.o,$(SOURCES)) $(RL_LIB_WEB) | $(D_OBJ_WEB_RELEASE)
	em++ $(patsubst $(D_SRC)/%.cpp,$(D_OBJ_WEB_RELEASE)/%.o,$(SOURCES)) $(WEB_LINK_FLAGS) -o $@

# ------------------------
# Raylib Library Build
# ------------------------
lib: $(RL_LIB)

ifeq ($(PLATFORM),windows)
# Windows: Download pre-built library from GitHub release based on submodule version.
$(RL_LIB): | lib/windows
	@echo "Downloading raylib $(RAYLIB_VERSION) for Windows (mingw-w64)..."
	@mkdir -p /tmp/raylib-win >/dev/null 2>&1
	@curl -L -o /tmp/raylib-win.zip https://github.com/raysan5/raylib/releases/download/$(RAYLIB_VERSION)/raylib-$(RAYLIB_VERSION)_win64_mingw-w64.zip >/dev/null 2>&1
	@echo "Extracting libraries to lib/windows/..."
	@unzip -q /tmp/raylib-win.zip "lib/*" -d /tmp/raylib-win >/dev/null 2>&1
	@cp -r /tmp/raylib-win/lib/* lib/windows/ >/dev/null 2>&1
	@rm -rf /tmp/raylib-win /tmp/raylib-win.zip
	@echo "Windows raylib $(RAYLIB_VERSION) setup complete!"

else ifeq ($(PLATFORM),linux)
# Linux: Build from source using raylib's Makefile with Wayland support.
$(RL_LIB): | lib/linux
	@echo "Building raylib $(RAYLIB_VERSION) for Linux..."
	@rm -f $(RL_SRC)/*.o $(RL_SRC)/libraylib.a
	@$(MAKE) -C $(RL_SRC) PLATFORM=PLATFORM_DESKTOP GLFW_LINUX_ENABLE_WAYLAND=TRUE GLFW_LINUX_ENABLE_X11=TRUE RAYLIB_LIBTYPE=STATIC >/dev/null 2>&1
	@cp $(RL_SRC)/libraylib.a $@
	@echo "Linux raylib $(RAYLIB_VERSION) built successfully!"

else ifeq ($(PLATFORM),web)
# Web: Build from source using raylib's Makefile.
$(RL_LIB): | lib/web
	@echo "Building raylib $(RAYLIB_VERSION) for Web..."
	@rm -f $(RL_SRC)/*.o $(RL_SRC)/libraylib.a
	@$(MAKE) -C $(RL_SRC) PLATFORM=PLATFORM_WEB CC=emcc AR=emar RAYLIB_LIBTYPE=STATIC >/dev/null 2>&1
	@cp $(RL_SRC)/libraylib.a $@
	@echo "Web raylib $(RAYLIB_VERSION) built successfully!"

endif

# Web library build (always available).
$(RL_LIB_WEB): | lib/web
	@echo "Building raylib $(RAYLIB_VERSION) for Web..."
	@rm -f $(RL_SRC)/*.o $(RL_SRC)/libraylib.a
	@$(MAKE) -C $(RL_SRC) PLATFORM=PLATFORM_WEB CC=emcc AR=emar RAYLIB_LIBTYPE=STATIC >/dev/null 2>&1
	@cp $(RL_SRC)/libraylib.a $@
	@echo "Web raylib $(RAYLIB_VERSION) built successfully!"

# ------------------------
# Directory Creation
# ------------------------
$(D_OBJ_DEBUG):
	mkdir -p $@

$(D_OBJ_RELEASE):
	mkdir -p $@

$(D_OBJ_WEB_DEBUG):
	mkdir -p $@

$(D_OBJ_WEB_RELEASE):
	mkdir -p $@

lib/windows:
	mkdir -p $@

lib/linux:
	mkdir -p $@

lib/web:
	mkdir -p $@

# ------------------------
# Utility Targets
# ------------------------
clean:
	rm -rf build lib

serve: $(D_OBJ_WEB_RELEASE)/index.html
	cd build/web/release && python -m http.server 8080

info:
	@echo "Native Platform: $(NATIVE_PLATFORM)"
	@echo "Build Directories:"
	@echo "  Native Debug:   $(D_OBJ_DEBUG)"
	@echo "  Native Release: $(D_OBJ_RELEASE)"
	@echo "  Web Debug:      $(D_OBJ_WEB_DEBUG)"
	@echo "  Web Release:    $(D_OBJ_WEB_RELEASE)"
