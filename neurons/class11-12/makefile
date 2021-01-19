APP=$(shell ls | grep cpp | sed s/.cpp//g)
all: $(APP)

$(APP): $(APP).o
	g++ $(APP).o -o $(APP) -ggdb

$(APP).o: $(APP).cpp
	g++ -std=c++14 $(APP).cpp -c -ggdb

clean:
	rm -f $(APP