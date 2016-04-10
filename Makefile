###########################################################################
#
#  Copyright (c) 2013-2015, ARM Limited, All Rights Reserved
#  SPDX-License-Identifier: Apache-2.0
#
#  Licensed under the Apache License, Version 2.0 (the "License"); you may
#  not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#  http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
#  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#
###########################################################################
# App
APP:=uvisor-mbed-helloworld

# Toolchain
PREFIX:=arm-none-eabi-
GDB:=$(PREFIX)gdb
OBJDUMP:=$(PREFIX)objdump

MBED_TARGET:=K64F
MBED_TOOLCHAIN:=GCC_ARM

# set JLINK CPU based on ARCH
ifeq ("$(MBED_TARGET)","K64F")
	CPU:=MK64FN1M0XXX12
else
	CPU:=UNKNOWN
endif

# JLink settings
JLINK:=$(SEGGER)JLinkExe
JLINK_CFG:=-Device $(CPU) -if SWD
JLINK_SERVER:=$(SEGGER)JLinkGDBServer
DEBUG_HOST:=localhost:2331

# Paths
TARGET:=./.build/$(MBED_TARGET)/$(MBED_TOOLCHAIN)/$(APP)
TARGET_ELF:=$(TARGET).elf
TARGET_BIN:=$(TARGET).bin
TARGET_ASM:=$(TARGET).asm

FW_DET:=MBED.HTM
FW_DIR:=$(wildcard /Volumes/*/$(FW_DET))
ifeq ("$(wildcard $(FW_DIR))","")
	FW_DIR:=$(wildcard /run/media/$(USER)/*/$(FW_DET))
endif
ifeq ("$(wildcard $(FW_DIR))","")
	FW_DIR:=$(wildcard /var/run/media/$(USER)/*/$(FW_DET))
endif
ifeq ("$(wildcard $(FW_DIR))","")
	FW_DIR:=./
else
	FW_DIR:=$(dir $(firstword $(FW_DIR)))
endif

# GDB scripts
include Makefile.scripts

# Read uVisor symbols.
UVISOR_LIB:=mbed-os/core/uvisor-mbed-lib
UVISOR_ELF:=$(UVISOR_LIB)/deploy/TARGET_IGNORE/uvisor/platform/kinetis/debug/configuration_kinetis_m4_0x1fff0000.elf
GDB_DEBUG_UVISOR:=add-symbol-file $(UVISOR_ELF) uvisor_init

.PHONY: all clean build objdump debug gdbserver

all: $(TARGET_BIN)

clean:
	rm -rf .build gdb.script
	make -C $(UVISOR_LIB) clean

install: $(TARGET_BIN)
	cp $^ $(FW_DIR)firmware.bin
	sync

build: $(TARGET_BIN)

$(TARGET_BIN): $(UVISOR_ELF)
	neo.py compile -o "debug-info" -t $(MBED_TOOLCHAIN) -m $(MBED_TARGET) -j 0

objdump: $(TARGET_ASM)

$(TARGET_ASM): $(TARGET_ELF)
	$(OBJDUMP) -d $^ > $@

gdbserver:
	$(JLINK_SERVER) $(JLINK_CFG)

$(UVISOR_ELF):
	make -C $(UVISOR_LIB) all

debug: gdb.script $(UVISOR_ELF) $(TARGET_BIN)
	$(GDB) -tui -x $<

gdb.script:
	@echo "$$__SCRIPT_GDB" > $@
