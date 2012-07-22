g++ -std=c++0x -I F:\Projects\libraries\c++\boost_1_49_0 -I F:\Projects\libraries\c++\glm-0.9.3.1 -c main.cpp -o glblox.o -lglew32 -lopengl32 -lglu32
g++ -shared -Wl,-soname,glblox.so -o glblox.so glblox.o -lglew32 -lopengl32 -lglu32 -static-libgcc -static-libstdc++
