CC=g++
CPPFLAGS=-lncursesw
TARGET=SnakeProject
OBJECTS= new_snake.o gamehandler.o point.o map.o snake.o item.o gate.o

all:$(TARGET)

$(TARGET):$(OBJECTS)
					$(CC) -o $@ $^ $(CPPFLAGS)

%.o:%.cpp
					$(CC) -c -o $@ $^ $(CPPFLAGS)
