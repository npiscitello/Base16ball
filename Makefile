# Makefile for base16ball

# All absolute paths are for the install on the Turing system
# They shouldn't hurt things on other systems as long as the
# include file and library can still be found

# Sources to build
SRCS := ball_testapp.cc \
				umpire_testapp.cc \
				main.cc
BINS := $(SRCS:.cc=)

# Compiler arguments
CXXFLAGS := -std=c++11

# Search paths for '*.h' files
INCLUDES := -I/home/piscitellon/custom_builds/install/include/ncurses

# Search paths for 'lib*.so' files
LD_PATHS := -L/home/piscitellon/custom_builds/install/lib

# Shared libraries to link against
LD_FLAGS := -lmenu \
						-lncurses

# Compile all binaries
all: $(BINS)

# Compile a binary from a '*.cc' source
%: %.cc
	@echo "compiling $@..."
	g++ $(CXXFLAGS) $(INCLUDES) $(LD_PATHS) -o $@ $^ $(LD_FLAGS)

# Clean all compiled things
.PHONY: clean
clean:
	@echo "Cleaning out compiled binaries..."
	rm -f $(BINS)
