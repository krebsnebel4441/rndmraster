CC=cc
TARGET=raster
OBJECTS=main.o

all: $(TARGET)

raster: $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS)

%.o: %.c
	$(CC) -c $< -o $@

.PHONY: clean

clean:
	rm -rf $(TARGET) $(OBJECTS)
