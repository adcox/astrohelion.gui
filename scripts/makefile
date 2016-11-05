# This file will compile various tests
#
# Syntax Notes
# 	$^ gives the RHS of the :
#	$@ gives the LHS of the :
#	$< gives the first item in the dependencies list, i.e. RHS of :
#
#	This code: $(patsubst %.o,$(OBJ)/%.o, $^)
#	replaces all the object %.o files in the dependency list with a relative path using the $(OBJ) macro

# Instructions for creating libraries:
# <http://www.iram.fr/~roche/code/c++/AddNumbers.html>

############################################################
# Macros for compiling
############################################################

# My headers
INC := ../include
# System headers; these are included to not throw warnings
INC_SYS  := ../include_extern /opt/local/include
# Source files
SRC := ../src
# Destination directory for compiled objects; use one for optimized, second for debug versions
# OBJ := ../obj
OBJ := ../obj_debug
# Directory for compiled binaries
BIN := ../bin
# Location of library dependencies
LIB := ../lib

# Compiler specification and flags
# CXX := clang++ -std=c++11
CXX := g++-6 -std=c++11
# CXX := g++-5 -std=c++11
CFLAGS += -ggdb -W -Wall -Wextra -Weffc++ -pedantic
# CFLAGS += -O3 -W -Wall -Wextra -Weffc++ -pedantic

# Library names and locations
LDFLAGS += -L $(LIB)
LIBS = glfw3 GLEW SOIL freetype
LDFLAGS += $(foreach lib, $(LIBS),-l$(lib))

# Options that are platform dependent
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
	LDFLAGS += -L /usr/local/lib
endif

ifeq ($(UNAME_S), Darwin)
	CFLAGS += -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo -framework Carbon -lGLEW
endif

COMP := $(CXX) $(CFLAGS)

# Get JUST the filenames, no filepaths, of the source files
SRC_FILES := $(notdir $(shell find $(SRC)/*.cpp))

# Get a list of all CPP files in SRC directory
SOURCES := $(addprefix $(SRC)/,$(SRC_FILES))

# Get list of all object files by copying source file names and 
# putting the OBJ path before the name
OBJECTS := $(patsubst %.cpp,$(OBJ)/%.o, $(SRC_FILES))

# Header files that don't have associated objects; we need the compiler to
# know that objects are dependent on these and to update if changes are made to them
IMPORTANT_HEADERS := 

# Append -isystem to all the system include paths
EXTRA_INC := $(addprefix -isystem ,$(INC_SYS))
INCLUDES := -I $(INC) $(EXTRA_INC)

HEADER_DEPS := $(addprefix $(INC)/,$(IMPORTANT_HEADERS))
############################################################
## Macros to make dependency lists shorter (don't have to put)
## OBJ in front of all the files, use a macro to do the substitution
############################################################


############################################################
.PHONY: printVars

all: 
	

############################################################
## TESTS - All executable %.out files go in the BIN directory
############################################################

arch: $(OBJECTS)
	$(COMP) $(INCLUDES) $^ $(LDFLAGS) structureTest.cpp -o $(BIN)/$@

coord: $(OBJECTS)
	$(COMP) $(INCLUDES) $^ $(LDFLAGS) coordTutorial.cpp -o $(BIN)/$@

firstTutorial: $(OBJECTS)
	$(COMP) $(INCLUDES) $^ $(LDFLAGS) firstTutorial.cpp -o $(BIN)/$@

font: $(OBJECTS)
	$(COMP) $(INCLUDES) $^ $(LDFLAGS) fontTest.cpp -o $(BIN)/$@

texture: $(OBJECTS)
	$(COMP) $(INCLUDES) $^ $(LDFLAGS) textureTutorial.cpp -o $(BIN)/$@

thickLines: $(OBJECTS)
	$(COMP) $(INCLUDES) $^ $(LDFLAGS) thickLines.cpp -o $(BIN)/$@

multiWindow: $(OBJECTS)
	$(COMP) $(INCLUDES) $^ $(LDFLAGS) multiWindowTest.cpp -o $(BIN)/$@
############################################################
## OBJECTS - All the %.o files go in the OBJ directory
############################################################

$(OBJ)/%.o: $(SRC)/%.cpp $(HEADER_DEPS)
	$(COMP) $(INCLUDES) -c $< -o $@

############################################################
## UTILITY
############################################################
clean:
	@- $(RM) $(OBJ)/*.o

cleandist: clean

nuke:
	@- $(RM) $(OBJ)/*.o
	@- $(RM) $(BIN)/*.out

printVars:
	$(info $(SRC_FILES))
	$(info $(SOURCES))
	$(info $(OBJECTS))

list:
	@- echo "You may choose from the following options:"
	@- echo " arch - Test my custom classes and definitions, i.e., my custom OpenGL architecture"
	@- echo " firstTutorial - Test first several tutorial's worth of code"
	@- echo " texture - Test the texture tutorial code"



