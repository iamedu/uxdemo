uxtest: main.cc util.cc shaders.cc shapes.cc vertex.cc texture.cc 
	g++ -L/opt/local/lib/ -I/opt/local/include -lfreetype -lSOIL -lglfw3 -Iinclude -I/usr/local/include/freetype2 -framework IOKit -framework CoreVideo -framework Cocoa -framework OpenGL main.cc util.cc shaders.cc shapes.cc vertex.cc texture.cc -o uxtest

clean: 
	rm uxtest
