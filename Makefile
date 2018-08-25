CC = gcc
SRC_DIR = src
SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/matrix.c $(SRC_DIR)/lu_decomposition.c
OBJS = $(SRCS:.c=.o) 
TARGET = lu_decompositon

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

$(BUILD_DIR)/%.o: %.c
	$(CC) -c -o $@ $<

clean:
	rm -rf $(OBJS) $(TARGET)
