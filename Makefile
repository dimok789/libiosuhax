#---------------------------------------------------------------------------------
# Clear the implicit built in rules
#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
ifeq ($(strip $(DEVKITPPC)),)
$(error "Please set DEVKITPPC in your environment. export DEVKITPPC=<path to>devkitPPC")
endif

export PATH			:=	$(DEVKITPPC)/bin:$(PORTLIBS)/bin:$(PATH)
export PORTLIBS		:=	$(DEVKITPRO)/portlibs/ppc

PREFIX	:=	powerpc-eabi-

export AS	:=	$(PREFIX)as
export CC	:=	$(PREFIX)gcc
export CXX	:=	$(PREFIX)g++
export AR	:=	$(PREFIX)ar
export OBJCOPY	:=	$(PREFIX)objcopy

include $(DEVKITPPC)/base_rules

#---------------------------------------------------------------------------------
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
#---------------------------------------------------------------------------------
BUILD		:=	build
SOURCES		:=	source
INCLUDES	:=	iosuhax.h iosuhax_devoptab.h iosuhax_disc_interface.h
LIBTARGET	:=	libiosuhax.a

#---------------------------------------------------------------------------------
# installation parameters
#---------------------------------------------------------------------------------
LIBINSTALL	:=	$(PORTLIBS)/lib
HDRINSTALL	:=	$(PORTLIBS)/include

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
CFLAGS		=	-O2 $(DEF_FLAGS) -Wall -Wno-unused $(MACHDEP) $(INCLUDE)
CXXFLAGS	=	$(CFLAGS)
ASFLAGS		:=	-g

#---------------------------------------------------------------------------------
# any extra libraries we wish to link with the project
#---------------------------------------------------------------------------------
LIBS		:=

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS		:=

#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------

export DEPSDIR	:=	$(CURDIR)/$(BUILD)

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
					$(foreach dir,$(DATA),$(CURDIR)/$(dir))

CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))


export OFILES	:=	$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)

export INCLUDE	:=	$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					-I$(CURDIR)/$(BUILD) -I$(LIBOGC_INC) \
					-I$(PORTLIBS)/include

export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib) \
					-L$(LIBOGC_LIB) -L$(PORTLIBS)/lib

.PHONY: $(BUILD) clean

#---------------------------------------------------------------------------------
$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------
all: $(LIBTARGET)

clean:
	@echo clean ...
	@rm -fr $(BUILD) $(LIBTARGET)

install: $(LIBTARGET)
	@[ -d $(LIBINSTALL) ] || mkdir -p $(LIBINSTALL)
	@[ -d $(HDRINSTALL) ] || mkdir -p $(HDRINSTALL)
	cp $(foreach incl,$(INCLUDES),$(SOURCES)/$(incl)) $(HDRINSTALL)
	cp $(LIBTARGET) $(LIBINSTALL)

wut:
	@[ -d $(BUILD) ] || mkdir -p $(BUILD)
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile DEF_FLAGS=-D__WUT__

#---------------------------------------------------------------------------------
else

DEPENDS	:=	$(OFILES:.o=.d)

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
$(LIBTARGET): $(OFILES)
	@rm -f "../$(LIBTARGET)"
	@$(AR) rcs "../$(LIBTARGET)" $(OFILES)
	@echo built $(notdir $@)

$(LIBDIR)/$(PLATFORM):
	mkdir -p ../$(LIBDIR)/$(PLATFORM)

-include $(DEPENDS)

#---------------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------------
