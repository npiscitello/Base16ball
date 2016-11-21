# Makefile for base16ball

# All absolute paths are for the install on the Turing system
# They shouldn't hurt things on other systems as long as the
# include file and library can still be found

# add custom-built binaries to the path for WIT's Turing server
PATH := /home/piscitellon/custom_builds/install/bin:$(PATH)

# Sources to build
SRCS := ball_testapp.cc \
				umpire_testapp.cc \
				scorereader.cc \
				main.cc
SRC := $(addprefix src/,$(SRCS))
BINS := $(SRCS:.cc=)
BINS := $(addprefix bin/,$(BINS))

# protobuf sources
PROTO_MSGS 	:= leaderboard.proto
PROTO_MSGS 	:= $(addprefix proto/, $(PROTO_MSGS))
PROTO_CPP		:= $(PROTO_MSGS:.proto=.pb.cc)

# Compiler arguments
CXXFLAGS := -std=c++11

# Search paths for '*.h' files
INCLUDES := -I. \
						-I./include \
						-I/home/piscitellon/custom_builds/install/include/ \
						-I/home/piscitellon/custom_builds/install/include/ncurses

# Search paths for 'lib*.so' files
LD_PATHS := -L/home/piscitellon/custom_builds/install/lib

# Shared libraries to link against
LD_FLAGS := -lmenu \
						-lncurses \
						-lprotobuf

# only compile the game essentials
.PHONY: game
game: base16ball
	@echo ""
	@echo "=========================="
	@echo " Game essentials compiled "
	@echo "=========================="
	@echo ""

# Compile all binaries
all: base16ball $(BINS)

# Compile a binary from a '*.cc' source
bin/%: src/%.cc $(PROTO_CPP)
	@echo -e "\ncompiling $@..."
	@mkdir -p bin
	g++ $(CXXFLAGS) $(INCLUDES) $(LD_PATHS) -o $@ $^ $(LD_FLAGS)

# Compile usable protobuf messages from .proto files
.PHONY: protobuf
protobuf: $(PROTO_CPP)
%.pb.h %.pb.cc: %.proto
	@echo -e "\ngenerating protobuf messages from $^"
	protoc --cpp_out=. $(PROTO_MSGS)

# make the full game (makes main and symlinks it to a file in the repo root)
base16ball: bin/main
	@ln -s $< base16ball

# Clean all compiled things
.PHONY: clean
clean:
	@echo -e "\nCleaning out compiled binaries..."
	rm -rf bin proto/*.pb.* base16ball
