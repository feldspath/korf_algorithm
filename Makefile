CPPFLAGS = -g -std=c++14 -Wall
CXX = g++

_TARGETS := solver

ODIR := build
SDIR := src

CPP_FILES := $(wildcard $(SDIR)/*.cpp)

OBJ_FILES := $(patsubst $(SDIR)/%.cpp, $(ODIR)/%.o,$(CPP_FILES))
TARGETS := $(patsubst %, $(ODIR)/%,$(_TARGETS))


#-------
# RULES 
#-------

all : directories $(TARGETS)

directories:
	mkdir -p $(ODIR)

clean: 
	rm -f -r $(ODIR)

$(ODIR)/% : $(OBJ_FILES)
	echo "Linking $@..."
	$(CXX) -g -o $@ $^ $(CPPFLAGS)

$(ODIR)/%.o : $(SDIR)/%.cpp
	echo "Compiling $<..."
	$(CXX) -g -o $@ -c $< $(CPPFLAGS)

.PHONY: all clean


# Comment this line to remove .o files after compilation
.SECONDARY: $(OBJ_FILES) $(patsubst %, %.o,$(TARGETS))