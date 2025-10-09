.PHONY: all native web clean serve

CXX       := clang++
CC        := clang
EMXX      := em++
EMCC	  := emcc
STD       := -std=c++23
WARNINGS  := -Wall -Wextra -Werror -Wshadow -Wnon-virtual-dtor -Wold-style-cast
INCLUDES  := -I. -Iinclude -Iexternal/raylib/src

# Directories
D_SRC     		:= src
D_OBJ_DEBUG 	:= build/native/debug
D_OBJ_RELEASE 	:= build/native/release
D_OBJ_WEB 		:= build/web
D_OBJ_RL  		:= build/raylib/web
D_OUT_WEB 		:= build/web/out

# Raylib sources
RL_SRC    := external/raylib/src
RL_OBJS   := $(D_OBJ_RL)/rcore.o \
             $(D_OBJ_RL)/rshapes.o \
             $(D_OBJ_RL)/rtextures.o \
			 $(D_OBJ_RL)/rtext.o \
			 $(D_OBJ_RL)/rmodels.o \
		     $(D_OBJ_RL)/raudio.o \
			 $(D_OBJ_RL)/utils.o
RL_LIB    := $(D_OBJ_RL)/libraylib_web.a

# Build flags
CXXFLAGS_NATIVE_DEBUG 	:= $(STD) $(WARNINGS) $(INCLUDES)
CXXFLAGS_NATIVE_RELEASE := $(STD) $(WARNINGS) $(INCLUDES) -DNDEBUG -O3
CXXFLAGS_WEB 			:= $(STD) $(WARNINGS) $(INCLUDES) -DPLATFORM_WEB -DNDEBUG -O3

# Native link flags
NATIVE_LINK_FLAGS := -lraylib -lm -ldl -lpthread -lGL

# Emscripten link flags
WEB_PRELOAD_ASSETS := $(shell find res -type f | xargs -I{} echo --preload-file {})
WEB_LINK_FLAGS := $(RL_LIB) \
                  -s USE_GLFW=3 -s ASYNCIFY -s ALLOW_MEMORY_GROWTH=1 \
				  -s FORCE_FILESYSTEM=1 $(WEB_PRELOAD_ASSETS) \
                  --shell-file web/shell.html

# ========================
# Rules
# ========================

all: native web

native: $(NATIVE_LINK_FLAGS) $(D_OBJ_DEBUG)/blinks_thinks $(D_OBJ_RELEASE)/blinks_thinks
web:    $(RL_LIB) $(D_OUT_WEB)/index.html

# ------------------------
# Native build
# ------------------------
$(D_OBJ_DEBUG)/%.o: $(D_SRC)/%.cpp | $(D_OBJ_DEBUG)
	$(CXX) $(CXXFLAGS_NATIVE_DEBUG) -c $< -o $@

$(D_OBJ_DEBUG)/blinks_thinks: $(patsubst $(D_SRC)/%.cpp,$(D_OBJ_DEBUG)/%.o,$(wildcard $(D_SRC)/*.cpp)) | $(D_OBJ_DEBUG)
	$(CXX) $^ $(NATIVE_LINK_FLAGS) -o $@

$(D_OBJ_RELEASE)/%.o: $(D_SRC)/%.cpp | $(D_OBJ_RELEASE)
	$(CXX) $(CXXFLAGS_NATIVE_RELEASE) -c $< -o $@

$(D_OBJ_RELEASE)/blinks_thinks: $(patsubst $(D_SRC)/%.cpp,$(D_OBJ_RELEASE)/%.o,$(wildcard $(D_SRC)/*.cpp)) | $(D_OBJ_RELEASE)
	$(CXX) $^ $(NATIVE_LINK_FLAGS) -o $@

# ------------------------
# Web build
# ------------------------
$(D_OBJ_WEB)/%.o: $(D_SRC)/%.cpp | $(D_OBJ_WEB)
	$(EMXX) $(CXXFLAGS_WEB) -c $< -o $@

$(D_OUT_WEB)/index.html: $(patsubst $(D_SRC)/%.cpp,$(D_OBJ_WEB)/%.o,$(wildcard $(D_SRC)/*.cpp)) | $(D_OUT_WEB)
	$(EMXX) $^ $(WEB_LINK_FLAGS) -o $@

# ------------------------
# Raylib
# ------------------------
$(D_OBJ_RL)/%.o: $(RL_SRC)/%.c | $(D_OBJ_RL)
	$(EMCC) -w -c $< -o $@ -DPLATFORM_WEB -DGRAPHICS_API_OPENGL_ES2

$(RL_LIB): $(RL_OBJS) | $(D_OBJ_RL)
	emar rcs $@ $^

# ------------------------
# Directory creation
# ------------------------
$(D_OBJ_DEBUG):
	mkdir -p $@

$(D_OBJ_RELEASE):
	mkdir -p $@

$(D_OBJ_WEB):
	mkdir -p $@

$(D_OBJ_RL_NATIVE):
	mkdir -p $@

$(D_OBJ_RL):
	mkdir -p $@

$(D_OUT_WEB):
	mkdir -p $@

# ------------------------
# Cleanup
# ------------------------
clean:
	rm -rf build

# ------------------------
# Host local web build
# ------------------------
serve: web
	cd $(D_OUT_WEB) && python -m http.server 8080
