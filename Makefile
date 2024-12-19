CC = gcc

# directory
SRC_DIR = src
BIN_DIR = bin
LIB_DIR = lib

# source
SRC = $(shell find $(SRC_DIR) -name '*.c')
OBJ = $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(SRC))
DEP = $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.d, $(SRC))

CFLAGS = -Wall -Wextra -Werror -I$(SRC_DIR)

DEPFLAGS = -MMD -MP

# output
EXE = app

# command
PRINT = echo
DEL = rm -rf
MKDIR = mkdir -p

CMAKE = cmake

# glfw
GLFW_DIR = $(LIB_DIR)/glfw
GLFW_BIN = $(BIN_DIR)/glfw
GLFW_LIB = $(GLFW_BIN)/src
GLFW_INC = $(GLFW_DIR)/include
GLFW_FLAGS += -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF
GLFW_FLAGS += -DGLFW_BUILD_DOCS=OFF
LDFLAGS += -L$(GLFW_LIB) -I$(GLFW_INC) -lglfw3 -lopengl32 -lgdi32

# vulkan
VULKAN_LIB = $(VULKAN_SDK)/Lib
VULKAN_INC = $(VULKAN_SDK)/Include
LDFLAGS += -L$(VULKAN_LIB) -I$(VULKAN_INC) -lvulkan-1
GLSLC = $(VULKAN_SDK)/Bin/glslc.exe

# shader
SHADER_DIR = shaders
SHADER_SRC = $(shell find $(SHADER_DIR) -name '*.vert' -o -name '*.frag')
SHADER_DST = $(patsubst $(SHADER_DIR)/%.vert, $(BIN_DIR)/%.vert.spv, $(SHADER_SRC)) \
			 $(patsubst $(SHADER_DIR)/%.frag, $(BIN_DIR)/%.frag.spv, $(SHADER_SRC))

all: glfw $(EXE) $(SHADER_DST)

$(EXE): $(OBJ)
	@$(PRINT) "Linking $@"
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c | $(BIN_DIR)
	@$(MKDIR) $(dir $@)
	@$(PRINT) "Compiling $<"
	@$(CC) $(CFLAGS) $(DEPFLAGS) -c -o $@ $< $(LDFLAGS)

$(BIN_DIR):
	@$(MKDIR) $@

$(BIN_DIR)/%.vert.spv: $(SHADER_DIR)/%.vert | $(BIN_DIR)
	@$(PRINT) "Compiling $<"
	@$(GLSLC) $< -o $@

$(BIN_DIR)/%.frag.spv: $(SHADER_DIR)/%.frag | $(BIN_DIR)
	@$(PRINT) "Compiling $<"
	@$(GLSLC) $< -o $@

submodule:
	@$(PRINT) "Initializing submodules"
	@git submodule update --init --recursive

glfw: submodule
	@$(CMAKE) -S $(GLFW_DIR) -B $(GLFW_BIN) $(GLFW_FLAGS)
	@$(CMAKE) --build $(GLFW_BIN)

clean:
	@$(DEL) $(OBJ) $(DEP) $(EXE)

clean-all: clean
	@$(DEL) $(BIN_DIR)

-include $(DEP)

.PHONY: all clean	
