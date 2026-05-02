# CXX — the C++ compiler to use
# CXXFLAGS — compiler flags (standard, include paths, defines)
# LDFLAGS — linker flags (library search paths)
# LIBS — libraries to link against
# $(VAR) — how you reference a variable

# Variables — define once, reuse everywhere
CXX = clang++
CXXFLAGS = -std=c++17 -I./include -DGL_SILENCE_DEPRECATION
LDFLAGS = -L./include/lib-arm64
LIBS = -lglfw3 -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

# Find all .cpp files and derive executable names from them
SRCS = $(wildcard *.cpp)
TARGETS = $(SRCS:.cpp=)

# Build all executables
all: $(TARGETS)

# Pattern rule: build any executable from its matching .cpp file
%: %.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $< -o $@ $(LIBS)

.PHONY: clean all
clean:
	rm -f $(TARGETS)
