DEBUG = FALSE

GCC := nspire-gcc
AS  := nspire-as
GXX := nspire-g++
LD  := nspire-ld
GENZEHN := genzehn

GCCFLAGS  := -Wall -W -marm
LDFLAGS   := 
ZEHNFLAGS := --name "test" --compress

ifeq ($(DEBUG),FALSE)
	GCCFLAGS += -O3#s
else
	GCCFLAGS += -O0 -g
endif

SRC_DIR := src
BIN_DIR := build

OBJS := $(patsubst $(SRC_DIR)/%.c,   $(BIN_DIR)/%.o, $(shell find $(SRC_DIR) -name \*.c))
OBJS += $(patsubst $(SRC_DIR)/%.cpp, $(BIN_DIR)/%.o, $(shell find $(SRC_DIR) -name \*.cpp))
OBJS += $(patsubst $(SRC_DIR)/%.S,   $(BIN_DIR)/%.o, $(shell find $(SRC_DIR) -name \*.S))
OBJS := $(strip $(OBJS))

OBJDIRS := $(sort $(dir $(OBJS)))
OBJDIRS := $(OBJDIRS:/=)  # remove trailing '/' from dirs
OBJDIRS := $(strip $(OBJDIRS))

EXE := test



.PHONY: all
all: $(EXE).tns

.PHONY: clean
clean:
	@echo Deleting all build products...
	@rm -f $(EXE).tns $(EXE).elf $(EXE).zehn
	@rm -rf $(BIN_DIR)

.PHONY: rebuild
rebuild: clean all


# recipes to make the bin directories
$(foreach dir,$(OBJDIRS),$(eval $(dir):;@echo Creating directory \'$(dir)\'...;mkdir -p $(dir)))

.SECONDEXPANSION:  # to enable containing directory as a prerequisite
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c   | $$(@D)
	@echo "---- Compiling \033[0;36m$<\033[0m"
	@$(GCC) $(GCCFLAGS) -c $< -o $@

$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp | $$(@D)
	@echo "---- Compiling \033[0;36m$<\033[0m"
	@$(GXX) $(GCCFLAGS) -c $< -o $@

$(BIN_DIR)/%.o: $(SRC_DIR)/%.S   | $$(@D)
	@echo "---- Assembling \033[0;36m$<\033[0m"
	@$(AS) -c $< -o $@

$(EXE).tns: $(OBJS)
	@echo ---- Linking...
	@$(LD) $^ -o $(EXE).elf $(LDFLAGS)
	@echo Making .tns file...
	@$(GENZEHN) --input $(EXE).elf --output $(EXE).zehn $(ZEHNFLAGS)
	@make-prg $(EXE).zehn $@
	@rm $(EXE).elf
	@rm $(EXE).zehn
