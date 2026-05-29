CXX = clang++
CXXFLAGS = -std=c++17 -O2 -pthread -I./include -DGL_SILENCE_DEPRECATION
LDFLAGS = -L./include/lib-arm64
LIBS = -lglfw3 -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

main: main.o render.o
	$(CXX) $(LDFLAGS) main.o render.o -o main $(LIBS)

main.o: main.cpp Vec.h Sphere.h
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

render.o: render.cpp Render_points.h Camera.h Sphere.h
	$(CXX) $(CXXFLAGS) -c render.cpp -o render.o

.PHONY: clean
clean:
	rm -f main main.o render.o
