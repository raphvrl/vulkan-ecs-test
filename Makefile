ifdef MSYSTEM
	CC = gcc

	RM = rm -rf
	CP = cp
	MKDIR = mkdir -p
	PRINT = echo

	TARGET = app.exe

	CMAKE = cmake -G "MSYS Makefiles"
	CMAKE_BUILD = cmake --build
else ifeq ($(OS), Linux)
	CC = gcc

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

DEPFLAGS = -MMD -MP
CFLAGS = -std=c23 -Wall -Wextra -Wpedantic -Werror
CFLAGS += -I$(SRC_DIR)

LDFLAGS = -L$(LIB_DIR)

# vulkan
ifndef VULKAN_SDK
	$(error "vulkan sdk not found")
endif

ifeq ($(OS), Windows_NT)
	VULKAN_INC := $(VULKAN_SDK)/Include
	VULKAN_LIB := $(VULKAN_SDK)/Lib
	GLSLC := $(VULKAN_SDK)/Bin/glslc

	LDFLAGS += -I$(VULKAN_INC) -L$(VULKAN_LIB) -lvulkan-1
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
CGTLF_INC := $(CGTLF_DIR)

LDFLAGS += -I$(CGTLF_INC)

# shader
SHADER_DIR := shaders
SHADER_BIN := $(BIN_DIR)/shaders
SHADER_SRC := $(shell find $(SHADER_DIR) -name "*.vert" -o -name "*.frag")
SHADER_SPV := $(patsubst $(SHADER_DIR)/%.vert, $(SHADER_BIN)/%.vert.spv, $(SHADER_SRC))
SHADER_SPV += $(patsubst $(SHADER_DIR)/%.frag, $(SHADER_BIN)/%.frag.spv, $(SHADER_SRC))

all: $(GLFW_BIN) $(TARGET) $(SHADER_SPV)

$(TARGET): $(OBJ)
	@$(PRINT) "Linking $@"
	@$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	@$(PRINT) "Compiling $<"
	@$(MKDIR) $(dir $@)
	@$(CC) -c -o $@ $< $(CFLAGS) $(DEPFLAGS) $(LDFLAGS)

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
	@git submodule update --init --recursive
	@touch $@

clean-all:
	@$(PRINT) "Cleaning all"
	@$(RM) $(BIN_DIR) $(TARGET)

clean:
	@$(PRINT) "Cleaning"
	@$(RM) $(TARGET) $(OBJ) $(DEP)

.PHONY: all clean clean-all
