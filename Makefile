CC = g++
MINGW = i686-w64-mingw32-g++
CPP = $(shell find . -name "*.cpp")
H = $(shell find . -name "*.hpp")
LDFLAGS = -I $$HOME/LD37/Terminal-Play/src -L $$HOME/LD37/Terminal-Play/build -lterminalplay

run: build
	./game

build: $(CPP) $(H)
	$(CC) -std=c++11 -static $(LDFLAGS) -c $(CPP) $(H)
	$(CC) *.o $(LDFLAGS) -o game -static
	rm -f $(shell find . -name "*.gch")

windows: $(CPP) $(H)
	$(MINGW) -std=c++11 -static $(LDFLAGS) -c $(CPP) $(H)
	$(MINGW) *.o $(LDFLAGS) -o game.exe

clean:
	rm -rf *.o
