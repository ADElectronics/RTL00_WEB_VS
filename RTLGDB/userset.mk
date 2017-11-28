#=============================================
# User defined
#=============================================

#SDK_PATH ?= ../SDKRTLA/USDK/
#GCC_PATH = d:/MCU/GNU_Tools_ARM_Embedded/6.2017-q1-update/bin/# + or set in PATH
#OPENOCD_PATH = d:/MCU/OpenOCD/bin/
#TOOLS_PATH ?= $(SDK_PATH)component/soc/realtek/8195a/misc/iar_utility/common/tools/
#FLASHER_TYPE ?= Jlink
#FLASHER_TYPE ?= OCD
#FLASHER_PATH ?= $(SDK_PATH)flasher/
#JLINK_PATH ?= D:/MCU/SEGGER/JLink_V612i/
#JLINK_GDBSRV ?= JLinkGDBServer.exe


#SDK_PATH = SDKV35a/
SDK_PATH = USDK/

TOOLS_PATH ?= $(SDK_PATH)component/soc/realtek/8195a/misc/iar_utility/common/tools/

#FLASHER_TYPE = cmsis-dap
FLASHER_TYPE = Jlink
FLASHER_SPEED = 3500
FLASHER_PATH = $(SDK_PATH)flasher/

#JLINK_PATH ?= C:/Data/JLink_V620a/
JLINK_GDBSRV ?= JLinkGDBServer.exe
