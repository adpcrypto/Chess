all:
	g++ -I src/include -L src/lib -o main main.cpp -l mingw32 -l SDL3 -l SDL3_image
	./main.exe
