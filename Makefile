ifeq ($(OS), Windows_NT)
	ifdef MSYSTEM
		CC = gcc
		CXX = g++
		AR = ar rcs

		RM = rm -fr
		MKDIR = mkdir -p
		PRINT = echo

		CMAKE = cmake -G "Unix Makefiles"
	else
		$(error "only support MSYS2 for Windows")
	endif
else
	$(error "Unsupported platform")
endif

SRC_DIR = src
BIN_DIR = bin
LIB_DIR = lib

SRC = $(shell find $(SRC_DIR) -name "*.c")

OBJ = $(SRC:$(SRC_DIR)/%.c=$(BIN_DIR)/%.o)
DEP = $(OBJ:.o=.d)

CFLAGS = -MMD -MP -I$(SRC_DIR)

-include $(DEP)

ifeq ($(OS), Windows_NT)
	TARGET = app.exe
else
	TARGET = app
endif

# CFLAGS += -Wall -Wextra -Werror

# GLFW
GLFW_DIR = $(LIB_DIR)/glfw
GLFW_BIN = $(BIN_DIR)/glfw
GLFW_INC = $(GLFW_DIR)/include
GLFW_LIB = $(GLFW_BIN)/src
GLFW_STAMP = $(GLFW_BIN)/.stamp

GLFW_FLAGS = -DGLFW_BUILD_DOCS=OFF \
			 -DGLFW_BUILD_TESTS=OFF \
			 -DGLFW_BUILD_EXAMPLES=OFF \
			 -DGLFW_BUILD_WAYLAND=OFF \
			 -DCMAKE_BUILD_TYPE=Release

CFLAGS += -I$(GLFW_INC)
LDFLAGS += -lglfw3

ifeq ($(OS), Windows_NT)
	GLFW_FLAGS += -DGLFW_BUILD_X11=OFF \
				  -DGLFW_BUILD_WIN32=ON

	LDFLAGS += -lgdi32 -luser32 -lshell32 -lkernel32
endif

# Vulkan
VULKAN_INC = $(VULKAN_SDK)/Include
VULKAN_LIB = $(VULKAN_SDK)/Lib

CFLAGS += -I$(VULKAN_INC)
LDFLAGS += -L$(VULKAN_LIB) -lvulkan-1

# cglm
CGLM_DIR = $(LIB_DIR)/cglm
CGLM_INC = $(CGLM_DIR)/include

CFLAGS += -I$(CGLM_INC)

# stb
STB_DIR = $(LIB_DIR)/stb
STB_INC = $(STB_DIR)

CFLAGS += -I$(STB_INC)

# cgltf
CGLTF_DIR = $(LIB_DIR)/cgltf

CFLAGS += -I$(CGLTF_DIR)

# CImGui
CIMGUI_DIR = $(LIB_DIR)/cimgui
IMGUI_DIR = $(CIMGUI_DIR)/imgui
BACKEND_DIR = $(IMGUI_DIR)/backends
CIMGUI_BIN = $(BIN_DIR)/cimgui
CIMGUI_STAMP = $(CIMGUI_BIN)/.stamp

CPP_SRC = $(CIMGUI_DIR)/cimgui.cpp \
		  $(IMGUI_DIR)/imgui.cpp \
		  $(IMGUI_DIR)/imgui_demo.cpp \
		  $(IMGUI_DIR)/imgui_draw.cpp \
		  $(IMGUI_DIR)/imgui_tables.cpp \
		  $(IMGUI_DIR)/imgui_widgets.cpp \
		  $(BACKEND_DIR)/imgui_impl_vulkan.cpp \
		  $(BACKEND_DIR)/imgui_impl_glfw.cpp

CPP_OBJ = $(CPP_SRC:%.cpp=$(BIN_DIR)/%.o)

CFLAGS += -I$(CIMGUI_DIR) \
	 	  -I$(IMGUI_DIR) \
		  -I$(BACKEND_DIR)

CXXFLAGS = $(CFLAGS) \
		   -fpermissive \
		   -DIMGUI_IMPL_API="extern \"C\"" \
		   -DCIMGUI_USE_GLFW \
		   -DCIMGUI_USE_VULKAN \
		   -DIMGUI_DISABLE_OBSOLETE_FUNCTIONS \
		   -x c++

LDFLAGS += -lstdc++

# shader
GLSLC = $(VULKAN_SDK)/Bin/glslc.exe
SHADER_DIR = shaders
SHADER_BIN = $(BIN_DIR)/shaders
SHADER_SRC = $(shell find $(SHADER_DIR) -name "*.vert" -o -name "*.frag")
SHADER_DST = $(SHADER_SRC:$(SHADER_DIR)/%.vert=$(SHADER_BIN)/%.vert.spv) \
			 $(SHADER_SRC:$(SHADER_DIR)/%.frag=$(SHADER_BIN)/%.frag.spv)

all: $(TARGET)

$(TARGET): $(OBJ) $(CPP_OBJ) $(SHADER_DST) | $(GLFW_STAMP)
	@$(PRINT) "Linking $@"
	@$(CXX) $(OBJ) $(CPP_OBJ) $(LDFLAGS) -o $@

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	@$(PRINT) "Compiling $<"
	@$(MKDIR) $(@D)
	@$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/%.o: %.cpp
	@$(PRINT) "Compiling $<"
	@$(MKDIR) $(@D)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(SHADER_BIN)/%.vert.spv: $(SHADER_DIR)/%.vert | $(BIN_DIR)
	@$(PRINT) "Compiling $<"
	@$(MKDIR) $(@D)
	@$(GLSLC) $< -o $@

$(SHADER_BIN)/%.frag.spv: $(SHADER_DIR)/%.frag | $(BIN_DIR)
	@$(PRINT) "Compiling $<"
	@$(MKDIR) $(@D)
	@$(GLSLC) $< -o $@

$(GLFW_STAMP): | $(BIN_DIR)
	@$(PRINT) "Building GLFW"
	@$(MKDIR) $(GLFW_BIN)
	@$(CMAKE) -S $(GLFW_DIR) -B $(GLFW_BIN) $(GLFW_FLAGS)
	@$(MAKE) -C $(GLFW_BIN)
	@$(shell touch $@)

$(BIN_DIR):
	@$(MKDIR) $(BIN_DIR)

clean-code:
	@$(PRINT) "Cleaning code"
	@$(RM) $(OBJ)
	@$(RM) $(DEP)
	@$(RM) $(CPP_OBJ)

clean-glfw:
	@$(PRINT) "Cleaning GLFW"
	@$(RM) $(GLFW_BIN)

clean-cimgui:
	@$(PRINT) "Cleaning CImGui"
	@$(RM) $(CIMGUI_BIN)

clean-lib:
	@$(PRINT) "Cleaning lib"
	@$(RM) $(GLFW_BIN)
	@$(RM) $(CIMGUI_BIN)

clean-all:
	@$(PRINT) "Cleaning all"
	@$(RM) $(BIN_DIR)

.PHONY: all clean clean-all