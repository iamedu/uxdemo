uxtest: main.cc
	g++ -lglut -lGLU -lGL -Iinclude --std=c++11 main.cc util.cc shaders.cc -o uxtest

clean: uxtest
	rm uxtest
