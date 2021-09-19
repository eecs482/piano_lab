CC=g++ -g -Wall -std=c++17

# List of source files for your disk scheduler
PIANO_SOURCES=piano_lab.cpp

# Generate the names of the disk scheduler's object files
PIANO_OBJS=${PIANO_SOURCES:.cpp=.o}

# Default to regular Linux library
LIBTHREAD = libthread.o

ifeq ($(OS),Windows_NT)     # is Windows_NT on XP, 2000, 7, Vista, 10...
    detected_OS := Windows
else
    detected_OS := $(shell uname)
endif
# Print out the detected OS
$(info Detected OS is: $(detected_OS))

# Use Mac library if on Mac
ifeq ($(detected_OS),Darwin)  # Important to not have a space after the comma
        LIBTHREAD = libthread_macos.o
					CC += -D_XOPEN_SOURCE
endif

all: piano

# Compile the disk scheduler and tag this compilation
piano: ${PIANO_OBJS} $(LIBTHREAD)
	./autotag.sh
	${CC} -o $@ $^ -ldl -pthread 

# Generic rules for compiling a source file to an object file
%.o: %.cpp
	${CC} -c $<
%.o: %.cc
	${CC} -c $<

clean:
	rm -f ${PIANO_OBJS} piano
