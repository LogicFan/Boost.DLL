run : main.elf libplugin.so
	./main.elf ./
main.elf : tutorial2.cpp
	g++ -std=c++17 tutorial2.cpp -lstdc++fs -ldl -o main.elf
libplugin.so : plugin.cpp
	g++ -std=c++17 plugin.cpp -c -fPIC -fvisibility=hidden -o plugin.o
	g++ -shared plugin.o -o libplugin.so
clean :
	rm *.elf *.o *.so