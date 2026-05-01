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

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
TARGET = physics_engine

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $@ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean all
clean:
	rm -f $(OBJS) $(TARGET)