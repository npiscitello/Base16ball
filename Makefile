# Makefile for base16ball

# All absolute paths are for the install on the Turing system
# They shouldn't hurt things on other systems as long as the
# include file and library can still be found

# Sources to build
SRCS := ball_testapp.cc \
				umpire_testapp.cc \
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

# Compile all binaries
all: $(BINS)

# Compile a binary from a '*.cc' source
bin/%: src/%.cc
	@echo -e "\ncompiling $@..."
	@echo "protobuf sources are prereqs for any binary build..."
	@make $(PROTO_CPP)
	@mkdir -p bin
	g++ $(CXXFLAGS) $(INCLUDES) $(LD_PATHS) -o $@ $^ $(PROTO_CPP) $(LD_FLAGS)

# Compile usable protobuf messages from .proto files
%.pb.h %.pb.cc: %.proto
	@echo "generating protobuf messages from $^"
	protoc --cpp_out=. $(PROTO_MSGS)

# Clean all compiled things
.PHONY: clean
clean:
	@echo "Cleaning out compiled binaries..."
	rm -rf bin proto/*.pb.*
