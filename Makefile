CC=cc
AR=ar
TARGET=rndmraster.a
INCLUDE=include
OBJECTS=src/context.o src/rasterization.o
DEMO=raster
DEMOOBJ=demo/main.o

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(AR) rcs $(TARGET) $(OBJECTS)

%.o: %.c
	$(CC) -c $< -o $@ -I $(INCLUDE)

demo: $(DEMOOBJ) $(TARGET)
	$(CC) -o $(DEMO) demo/main.c $(TARGET) -I $(INCLUDE)

.PHONY: clean

clean:
	rm -rf $(TARGET) $(OBJECTS) $(DEMO) $(DEMOOBJ)
