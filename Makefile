CPPFLAGS=$(shell sdl2-config --cflags) $(-std=c++11 -Wall)
LDLIBS=$(shell sdl2-config --libs) -lGLEW $(EXTRA_LDLIBS)
EXTRA_LDLIBS?=-lGL
all: 
	g++ triangle.cpp $(CPPFLAGS) $(LDLIBS) -o triangle
clean:
	rm -f *.o triangle
.PHONY:
	all clean
