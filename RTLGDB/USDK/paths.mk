
# Compilation tools
CROSS_COMPILE = $(GCC_PATH)arm-none-eabi-
AR = $(CROSS_COMPILE)ar
CC = $(CROSS_COMPILE)gcc
AS = $(CROSS_COMPILE)as
NM = $(CROSS_COMPILE)nm
LD = $(CROSS_COMPILE)gcc
GDB = $(CROSS_COMPILE)gdb
SIZE = $(CROSS_COMPILE)size
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump

# TARGET dirs
TARGET ?= Debug
OBJ_DIR ?= $(TARGET)/obj
BIN_DIR ?= $(TARGET)/bin
ELFFILE ?= $(OBJ_DIR)/$(TARGET).axf

# Make bunary tools
ifneq ($(shell uname), Linux)
EXE = .exe
endif
PICK = $(TOOLS_PATH)pick$(EXE)
PADDING = $(TOOLS_PATH)padding$(EXE)
CHCKSUM = $(TOOLS_PATH)checksum$(EXE)

# openocd tools
OPENOCD = $(OPENOCD_PATH)openocd

