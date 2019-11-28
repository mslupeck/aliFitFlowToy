#---------------=============== ROOT  RELATED ===============---------------
ROOTCONFIG   := root-config

ARCH         := $(shell $(ROOTCONFIG) --arch)
PLATFORM     := $(shell $(ROOTCONFIG) --platform)
ALTCC        := $(shell $(ROOTCONFIG) --cc)
ALTCXX       := $(shell $(ROOTCONFIG) --cxx)
ALTF77       := $(shell $(ROOTCONFIG) --f77)
ALTLD        := $(shell $(ROOTCONFIG) --ld)

#CXX           =
ObjSuf        = o
SrcSuf        = cxx
ExeSuf        =
DllSuf        = so
OutPutOpt     = -o # keep whitespace after "-o"

ifeq (debug,$(findstring debug,$(ROOTBUILD)))
OPT           = -g
OPT2          = -g
else
ifneq ($(findstring debug, $(strip $(shell $(ROOTCONFIG) --config))),)
OPT           = -g
OPT2          = -g
else
OPT           = -O
OPT2          = -O2
endif
endif

ROOTCFLAGS   := $(shell $(ROOTCONFIG) --cflags)
ROOTLDFLAGS  := $(shell $(ROOTCONFIG) --ldflags)
ROOTLIBS     := $(shell $(ROOTCONFIG) --libs)
ROOTGLIBS    := $(shell $(ROOTCONFIG) --glibs)
HASTHREAD    := $(shell $(ROOTCONFIG) --has-thread)
ROOTDICTTYPE := $(shell $(ROOTCONFIG) --dicttype)
#NOSTUBS      := $(shell $(ROOTCONFIG) --nostubs)
ROOTCINT     := rootcint

# Stub Functions Generation
ifeq ($(NOSTUBS),yes)
   ROOTCINT = export CXXFLAGS="$(CXXFLAGS)"; $(ROOTSYS)/core/utils/src/rootcint_nostubs.sh -$(ROOTDICTTYPE)
endif

ifeq ($(ARCH),linuxx8664gcc)
# Linux with egcs, gcc 2.9x, gcc 3.x
CXX           = g++
CXXFLAGS      = $(OPT2) -Wall -fPIC
LD            = g++
LDFLAGS       = $(OPT2) -lTMVA -lMLP -lXMLIO -lTreePlayer -L/usr/local/lib -lnsl -lz -lm
SOFLAGS       = -shared
endif

CXXFLAGS     += $(ROOTCFLAGS)
LDFLAGS      += $(ROOTLDFLAGS)
LIBS          = $(ROOTLIBS) $(SYSLIBS)
GLIBS         = $(ROOTGLIBS) $(SYSLIBS)

ifneq ($(ALTCC),)
   CC  = $(ALTCC)
endif
ifneq ($(ALTCXX),)
   CXX = $(ALTCXX)
endif
ifneq ($(ALTF77),)
   F77 = $(ALTF77)
endif
ifneq ($(ALTLD),)
   LD  = $(ALTLD)
endif

ifneq ($(findstring g++, $(CXX)),)
GCC_MAJOR := $(shell $(CXX) -dumpversion 2>&1 | cut -d'.' -f1)
GCC_MINOR := $(shell $(CXX) -dumpversion 2>&1 | cut -d'.' -f2)
endif

CFLAGS	= -Wall -ggdb $(CXXFLAGS) $(LDFLAGS) $(ROOTCFLAGS) $(ROOTLDFLAGS) $(ROOTLIBS) $(ROOTGLIBS)
#---------------=============== ROOT  RELATED ===============---------------

#---------------=============== MAIN MAKEFILE ===============---------------
CC		= g++
TARGET	= aliFitFlowToy
SOURCE	= aliFitFlowToy.cpp DrawFuncs.cpp TCell.cpp TRing.cpp TDetector.cpp TEvent.cpp THarm.cpp THit.cpp TInputParams.cpp TInputCentralityClass.cpp cliParser.cpp cliParams.cpp

INCLUDE	= -I./ $(AN_INCLUDE_DIR)  
## end more includes

OBJ=$(join $(addsuffix obj/, $(dir $(SOURCE))), $(notdir $(SOURCE:.cpp=.o))) 

## Fix dependency destination to be ../.dep relative to the src dir
DEPENDS=$(join $(addsuffix .dep/, $(dir $(SOURCE))), $(notdir $(SOURCE:.cpp=.d)))

## Default rule executed
all: $(TARGET)
	@true

## Clean Rule
clean:
	@echo "Removing .o files"
	@-rm -f $(TARGET) $(OBJ) $(DEPENDS)
	@echo "Removing obj folders"
	@-rmdir obj
	@echo "Removing .dep folders"
	@-rmdir .dep

## Rule for making the actual target
$(TARGET): $(OBJ)
	@echo "============="
	@echo "Linking the target $@"
	@echo "============="
	@echo $(CC) $(CFLAGS) -o $@ $^ $(LIBS)
	@$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
	@echo -- Link finished --

## Generic compilation rule
%.o : %.cpp
	@mkdir -p $(dir $@)
	@echo "============="
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) -c $< -o $@


## Rules for object files from cpp files
## Object file for each file is put in obj directory
obj/%.o : %.cpp
	@mkdir -p $(dir $@)
	@echo "============="
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) -c $< -o $@

## Make dependancy rules
.dep/%.d: %.cpp
	@mkdir -p $(dir $@)
	@echo "============="
	@echo Building dependencies file for $*.o
	@$(SHELL) -ec '$(CC) -M $(CFLAGS) $< | sed "s^$*.o^obj/$*.o^" > $@'

## Include the dependency files
-include $(DEPENDS)
#---------------=============== MAIN MAKEFILE ===============---------------
