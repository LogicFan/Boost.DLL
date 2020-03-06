run : main.elf libplugin1.so libplugin2.so
	./main.elf ./plugin1 ./plugin2
main.elf : tutorial3.cpp
	g++ -std=c++17 tutorial3.cpp -lstdc++fs -ldl -o main.elf
libplugin1.so : plugin1.cpp
	g++ -std=c++17 plugin1.cpp -c -fPIC -fvisibility=hidden -o plugin1.o
	g++ -shared plugin1.o -o libplugin1.so
libplugin2.so : plugin2.cpp
	g++ -std=c++17 plugin2.cpp -c -fPIC -fvisibility=hidden -o plugin2.o
	g++ -shared plugin2.o -o libplugin2.so
clean :
	rm *.elf *.o *.so