APP=$(shell ls | grep cpp | sed s/.cpp//g)
all: $(APP)

$(APP): $(APP).o
	g++ $(APP).o -o $(APP) `pkg-config --libs opencv4` -ggdb

$(APP).o: $(APP).cpp
	g++ -std=c++14 `pkg-config --cflags opencv4` $(APP).cpp -c -ggdb

clean:
	rm -f $(APP