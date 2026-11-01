# ==============================================================================
# VARIABLES (CENTRALIZED CONFIGURATION)
# ==============================================================================

# Directory where object files (.o) and dependency files (.d) are placed.
BUILD_DIR := build

EXECUTABLE_NAME := baseconv
# ':=' assigns immediately. '=' assigns dynamically when accessed.
# TARGET is the primary executable required at the end of the build.
TARGET := $(BUILD_DIR)/$(EXECUTABLE_NAME)

# Directory containing source implementation (.c) files.
SRC_DIR := src

# Directory containing interface header (.h) files.
INC_DIR := include

# Root installation directory
PREFIX ?= /usr/local

# Toolchain compiler driver.
CC := gcc

# Flags for compiling .c source files into .o object files.
# -Wall -Wextra: Enable core and diagnostic warnings.
# -pedantic: Enforce strict ISO C standards.
# -std=c99: Language specification standard.
# -I$(INC_DIR): Search path for quoted header files.
# -O3: Maximum performance optimization.
# -MMD: Generate .d dependency files tracking included headers.
# -MP: Add phony rules for headers to avoid errors if renamed or deleted.
CFLAGS = -Wall -Wextra -pedantic -std=c99 -I$(INC_DIR) -O3 -MMD -MP

# Flags passed to the linker driver (e.g., library search directories).
LDFLAGS = -Wall -Wextra -std=c99 -O3

# Automatically discover all .c files and translate to matching .o paths.
# wildcard: Expands pattern to space-separated list of existing files.
# patsubst: Replaces matches of pattern 1 with pattern 2 in text.
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# ==============================================================================
# PHONY TARGETS
# ==============================================================================

# .PHONY tells make these targets are actions rather than real disk files.
# Prevents collisions if a file with the target name exists in the folder.
.PHONY: all clean run install uninstall

# ==============================================================================
# BUILD RULES
# ==============================================================================

# Default goal: First rule listed. Runs when make is invoked without arguments.
all: $(TARGET)

# Pattern Rule: Compiles single .c files into .o files.
# Runs on one specific source-target pair matching the pattern at a time.
# Evaluated for as many pairs as are out of date and required by the goal.
# $< = First prerequisite (.c file).
# $@ = Current target (.o file).
# $^ = All prerequisites.
# -c = Compile to machine code without linking.
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Linking Rule: Glues all compiled .o files into the final executable.
# Libraries (LDLIBS) must follow object files ($^) so the linker resolves
# symbols from left to right. Search paths (LDFLAGS) precede inputs.
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

# ==============================================================================
# UTILITY RULES
# ==============================================================================

# Convenience target: Builds dependencies if needed, then executes binary.
run: $(TARGET)
	./$(TARGET)

# Recursively removes the build directory and all generated artifacts.
clean:
	rm -rf $(BUILD_DIR)

#First ensures the root installation directory $(PREFIX) exists, then copies
#the final executable into it with permissions set to 755.
install: $(TARGET)
	install -d $(DESTDIR)$(PREFIX)/bin
	install -m 755 $(TARGET) $(DESTDIR)$(PREFIX)/bin/$(EXECUTABLE_NAME)

#Uninstalls the executable from the root installation directory
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(EXECUTABLE_NAME)

# ==============================================================================
# HEADER DEPENDENCY INGESTION
# ==============================================================================

# Ingest compiler-generated .d dependency files.
# Leading '-' silences errors if files do not exist yet (e.g., on clean build).
# Ingested rules (e.g., obj.o: src.c head.h) merge prerequisites into the
# pattern rule target, ensuring header changes trigger recompilation.
-include $(OBJS:.o=.d)
