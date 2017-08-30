##### Basic Configuration #####

EXE  := test
NAME := test

DEBUG := FALSE

SRC_DIR := src
BIN_DIR := build
WIN_DIR := /mnt/c/Users/Quinn.Tucker18/Downloads/temporary/ndless/


##### Variables #####

# build commands
GCC := nspire-gcc
AS  := nspire-as
GXX := nspire-g++
LD  := nspire-ld
GENZEHN := genzehn

# build flags
GCCFLAGS  := -Wall -W -marm
LDFLAGS   := 
ZEHNFLAGS := --name "$(NAME)" --compress

ifeq ($(DEBUG),FALSE)
	GCCFLAGS += -O3#s
else
	GCCFLAGS += -O0 -g
endif

# automatic dependency generation stuff
DEPFILE = $(@:.o=.d)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPFILE).tmp
FINISH_DEP = @mv -f $(DEPFILE).tmp $(DEPFILE) && touch $@

# list of object files to be built
OBJS := $(patsubst $(SRC_DIR)/%.c,   $(BIN_DIR)/%.o, $(shell find $(SRC_DIR) -name \*.c))
OBJS += $(patsubst $(SRC_DIR)/%.cpp, $(BIN_DIR)/%.o, $(shell find $(SRC_DIR) -name \*.cpp))
OBJS += $(patsubst $(SRC_DIR)/%.S,   $(BIN_DIR)/%.o, $(shell find $(SRC_DIR) -name \*.S))
OBJS := $(strip $(OBJS))

# list of build directories to be created
OBJDIRS := $(sort $(dir $(OBJS)))
OBJDIRS := $(OBJDIRS:/=)  # remove trailing '/' from dirs
OBJDIRS := $(strip $(OBJDIRS))

# list of dependency files to create/include
DEPFILES := $(OBJS:.o=.d)


##### Recipes #####

## Phonies ##

.PHONY: all
all: $(EXE).tns

.PHONY: clean
clean:
	@echo Deleting all build products...
	@rm -f $(EXE).tns $(EXE).elf $(EXE).zehn
	@rm -rf $(BIN_DIR)

.PHONY: rebuild
rebuild: clean all

## Regular recipes ##

# recipes to make the bin directories
$(foreach dir,$(OBJDIRS),$(eval $(dir):;@echo Creating directory \'$(dir)\'...;mkdir -p $(dir)))

# empty recipes to silence errors about missing .d files
$(foreach dep,$(DEPFILES),$(eval $(dep): ;))
.PRECIOUS: $(DEPFILES)

.SECONDEXPANSION:  # to enable the use of automatic vars in prerequisites
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c   $(BIN_DIR)/%.d | $$(@D)
	@echo "---- Compiling \033[0;36m$<\033[0m"
	@$(GCC) $(GCCFLAGS) $(DEPFLAGS) -c $< -o $@
	@$(FINISH_DEP)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp $(BIN_DIR)/%.d | $$(@D)
	@echo "---- Compiling \033[0;36m$<\033[0m"
	@$(GXX) $(GCCFLAGS) $(DEPFLAGS) -c $< -o $@
	@$(FINISH_DEP)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.s   $(BIN_DIR)/%.d | $$(@D)
	@echo "---- Assembling \033[0;36m$<\033[0m"
	$(warning This Makefile has not been tested for *.s sources)
	@$(AS) -c $< -o $@

$(EXE).tns: $(OBJS)
	@echo ---- Linking...
	@$(LD) $^ -o $(EXE).elf $(LDFLAGS)
	@echo Making .tns file...
	@$(GENZEHN) --input $(EXE).elf --output $(EXE).zehn $(ZEHNFLAGS)
	@make-prg $(EXE).zehn $@
	@rm $(EXE).elf
	@rm $(EXE).zehn
	@echo Copying to wund0ze...
	@cp $@ $(WIN_DIR)


# include the generated dependency files
include $(wildcard $(DEPFILES))
