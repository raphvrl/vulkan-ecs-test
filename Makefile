ifdef MSYSTEM
	CC = gcc
	CXX = g++
	AR = ar rcs

	RM = rm -rf
	CP = cp
	MKDIR = mkdir -p
	PRINT = echo

	TARGET = app.exe

	CMAKE = cmake -G "MSYS Makefiles"
	CMAKE_BUILD = cmake --build
else ifeq ($(OS), Linux)
	CC = gcc
	CXX = g++
	AR = ar rcs

	RM = rm -rf
	CP = cp
	MKDIR = mkdir -p
	PRINT = echo

	TARGET = app

	CMAKE = cmake -G "Unix Makefiles"
	CMAKE_BUILD = cmake --build
else
	$(error "Unsupported OS")
endif

SRC_DIR = src
BIN_DIR = bin
LIB_DIR = lib

SRC = $(shell find $(SRC_DIR) -name "*.c")
OBJ = $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(SRC))
DEP = $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.d, $(SRC))

CFLAGS = -std=c23 -Wall -Wextra -Wpedantic -Werror
CFLAGS += -I$(SRC_DIR)

LDFLAGS = -L$(LIB_DIR)

SUBMODULES = $(BIN_DIR)/.submodules

# vulkan
ifndef VULKAN_SDK
	$(error "vulkan sdk not found")
endif

ifeq ($(OS), Windows_NT)
	VULKAN_INC := "$(VULKAN_SDK)/Include"
	VULKAN_LIB := "$(VULKAN_SDK)/Lib"
	GLSLC := $(VULKAN_SDK)/Bin/glslc

	VULKAN_FLAGS = -I$(VULKAN_INC) -L$(VULKAN_LIB) -lvulkan-1
else ifeq ($(OS), Linux)
	LDFLAGS += -lvulkan
endif

# glfw
GLFW_DIR := $(LIB_DIR)/glfw
GLFW_INC := $(GLFW_DIR)/include
GLFW_BIN := $(BIN_DIR)/glfw
GLFW_LIB := $(GLFW_BIN)/src
GLFW_FLAGS += -DGLFW_BUILD_DOCS=OFF
GLFW_FLAGS += -DGLFW_BUILD_TESTS=OFF
GLFW_FLAGS += -DGLFW_BUILD_EXAMPLES=OFF

LDFLAGS += -I$(GLFW_INC) -L$(GLFW_LIB) -lglfw3

ifeq ($(OS), Windows_NT)
	LDFLAGS += -lgdi32
else ifeq ($(OS), Linux)
	LDFLAGS += -lX11 -lXrandr -lXinerama -lXi -lXcursor
endif

# cglm
GLM_DIR := $(LIB_DIR)/cglm
GLM_INC := $(GLM_DIR)/include

LDFLAGS += -I$(GLM_INC)

# stb
STB_DIR := $(LIB_DIR)/stb
STB_INC := $(STB_DIR)

LDFLAGS += -I$(STB_INC)

# cgltf
CGTLF_DIR := $(LIB_DIR)/cgltf

LDFLAGS += -I$(CGTLF_DIR)

# cimgui
CIMGUI_DIR := $(LIB_DIR)/cimgui
IMGUI_DIR := $(CIMGUI_DIR)/imgui
IMGUI_BACKEND_DIR := $(IMGUI_DIR)/backends

CIMGUI_BIN := $(BIN_DIR)/cimgui

CIMGUI_SRC := $(CIMGUI_DIR)/cimgui.cpp \
			  $(CIMGUI_DIR)/imgui/imgui.cpp \
			  $(CIMGUI_DIR)/imgui/imgui_draw.cpp \
			  $(CIMGUI_DIR)/imgui/imgui_demo.cpp \
			  $(CIMGUI_DIR)/imgui/imgui/imgui_widgets.cpp \
			  $(CIMGUI_DIR)/imgui/imgui/imgui_tables.cpp \
			  $(CIMGUI_DIR)/imgui/backends/imgui_impl_glfw.cpp \
			  $(CIMGUI_DIR)/imgui/backends/imgui_impl_vulkan.cpp

CIMGUI_OBJ := $(CIMGUI_BIN)/cimgui.o \
			  $(CIMGUI_BIN)/imgui/imgui.o \
			  $(CIMGUI_BIN)/imgui/imgui_draw.o \
			  $(CIMGUI_BIN)/imgui/imgui_demo.o \
			  $(CIMGUI_BIN)/imgui/imgui_widgets.o \
			  $(CIMGUI_BIN)/imgui/imgui_tables.o \
			  $(CIMGUI_BIN)/imgui/backends/imgui_impl_glfw.o \
			  $(CIMGUI_BIN)/imgui/backends/imgui_impl_vulkan.o

CIMGUI_FLAGS = -I$(CIMGUI_DIR) \
               -I$(IMGUI_DIR) \
               -I$(IMGUI_BACKEND_DIR) \
               -I$(VULKAN_INC) \
               -I$(GLFW_INC) \
               -DCIMGUI_NO_EXPORT \
               -DIMGUI_IMPL_API="extern \"C\"" \
               -DVK_USE_PLATFORM_WIN32_KHR \
               -DGLFW_INCLUDE_VULKAN \
               -DCIMGUI_USE_GLFW \
               -DCIMGUI_USE_VULKAN \
               -DIMGUI_DISABLE_OBSOLETE_FUNCTIONS \
               -DImTextureID="void*" \
               -DImDrawIdx="unsigned short" \
               -fPIC \
               -fpermissive \
               -Wno-error

CIMGUI_LDFLAGS = -L"$(VULKAN_LIB)" -lvulkan-1 \
				 -L$(GLFW_LIB) -lglfw3

CIMGUI_TARGET := $(CIMGUI_BIN)/libcimgui.a

LDFLAGS += -L$(CIMGUI_BIN) -lcimgui -I$(CIMGUI_DIR) -I$(IMGUI_DIR) -I$(IMGUI_BACKEND_DIR)

# shader
SHADER_DIR := shaders
SHADER_BIN := $(BIN_DIR)/shaders
SHADER_SRC := $(shell find $(SHADER_DIR) -name "*.vert" -o -name "*.frag")
SHADER_SPV := $(patsubst $(SHADER_DIR)/%.vert, $(SHADER_BIN)/%.vert.spv, $(SHADER_SRC))
SHADER_SPV += $(patsubst $(SHADER_DIR)/%.frag, $(SHADER_BIN)/%.frag.spv, $(SHADER_SRC))

LDFLAGS += $(VULKAN_FLAGS)

all: $(GLFW_BIN) $(CIMGUI_TARGET) $(TARGET) $(SHADER_SPV)

$(TARGET): $(OBJ)
	@$(PRINT) "Linking $@"
	@$(CXX) -o $@ $^ $(CFLAGS) $(LDFLAGS)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	@$(PRINT) "Compiling $<"
	@$(MKDIR) $(dir $@)
	@$(CC) -MMD -MP -c -o $@ $< $(CFLAGS) $(LDFLAGS)

-include $(DEP)

$(SHADER_BIN)/%.vert.spv: $(SHADER_DIR)/%.vert
	@$(PRINT) "Compiling $<"
	@$(MKDIR) $(dir $@)
	@$(GLSLC) $< -o $@

$(SHADER_BIN)/%.frag.spv: $(SHADER_DIR)/%.frag
	@$(PRINT) "Compiling $<"
	@$(MKDIR) $(dir $@)
	@$(GLSLC) $< -o $@

$(GLFW_BIN): $(SUBMODULES)
	@$(PRINT) "Building glfw"
	@$(MKDIR) $(GLFW_BIN)
	@$(CMAKE) -S $(GLFW_DIR) -B $(GLFW_BIN) $(GLFW_FLAGS)
	@$(CMAKE_BUILD) $(GLFW_BIN)

$(SUBMODULES):
	@$(PRINT) "Initializing submodules"
	@$(MKDIR) $(BIN_DIR)
	@git submodule update --init --recursive
	@touch $@

$(CIMGUI_BIN)/%.o: $(CIMGUI_DIR)/%.cpp
	@$(PRINT) "Compiling $<"
	@$(MKDIR) $(dir $@)
	@$(CXX) -c -o $@ $< $(CIMGUI_FLAGS) $(CIMGUI_LDFLAGS)

$(CIMGUI_TARGET): $(CIMGUI_OBJ)
	@$(PRINT) "Building cimgui"
	@$(MKDIR) $(dir $@)
	@$(AR) $@ $^

clean-all:
	@$(MAKE) -C $(CIMGUI_DIR) fclean
	@$(PRINT) "Cleaning all"
	@$(RM) $(BIN_DIR) $(TARGET)

clean:
	@$(PRINT) "Cleaning"
	@$(RM) $(TARGET) $(OBJ) $(DEP)

.PHONY: all clean clean-all
