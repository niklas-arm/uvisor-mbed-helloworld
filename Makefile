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

# JLink settings
JLINK:=$(SEGGER)JLinkExe
JLINK_CFG:=-Device $(CPU) -if SWD
CPU="MK64FN1M0XXX12"
JLINK_SERVER:=$(SEGGER)JLinkGDBServer
DEBUG_HOST:=localhost:2331

# Paths
UVISOR_ELF:=$(wildcard ./mbed-os/core/uvisor-mbed-lib/deploy/TARGET_IGNORE/uvisor/platform/kinetis/debug/*.elf)
TARGET:=./.build/$(APP)
TARGET_ELF:=$(TARGET).elf
TARGET_BIN:=$(TARGET).bin

# GDB scripts
include Makefile.scripts

# Read uVisor symbols.
GDB_DEBUG_UVISOR:=add-symbol-file $(UVISOR_ELF) uvisor_init

.PHONY: all build gdbserver gdb

all: gdb

$(TARGET_BIN):
	neo.py compile -o "debug-info" -t GCC_ARM -m K64F -j 0

gdbserver:
	$(JLINK_SERVER) $(JLINK_CFG)

gdb: gdb.script $(TARGET_BIN)
	$(GDB) -x $<

gdb.script:
	@echo "$$__SCRIPT_GDB" > $@
