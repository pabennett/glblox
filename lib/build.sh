g++ -std=c++0x -fPIC -c main.cpp -o glblox.o `pkg-config glew,glu --libs`
g++ -fPIC -shared -Wl,-soname,glblox.so -o glblox.so glblox.o `pkg-config --cflags --libs glew,glu`

