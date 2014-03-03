uxtest: main.cc util.cc shaders.cc shapes.cc vertex.cc texture.cc download.cc json.cpp
	g++ -I/opt/local/include /usr/local/lib/liblua.a /usr/local/lib/libfreetype.a /opt/local/lib/libSOIL.a /usr/local/lib/libglfw3.a -lcurl -Iinclude -I/usr/local/include/freetype2 -framework IOKit -framework CoreVideo -framework Cocoa -framework OpenGL main.cc util.cc shaders.cc shapes.cc vertex.cc texture.cc script.cc download.cc json.cpp -o uxtest block_allocator.cpp

clean: 
	rm uxtest
