# Compiler and flags
CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -std=c99 -lm

# File paths
CANVAS_SRC = src/canvas.c
CANVAS_DEMO = demo/main.c
CANVAS_OUT = canvas_demo

MATH_SRC = src/math3d.c
MATH_DEMO = demo/test_math.c
MATH_OUT = test_math

.PHONY: all clean run_canvas run_math

# Default build all demos
all: $(CANVAS_OUT) $(MATH_OUT)

# Canvas demo
$(CANVAS_OUT): $(CANVAS_SRC) $(CANVAS_DEMO)
	$(CC) $(CFLAGS) $^ -o $@

# Math demo
$(MATH_OUT): $(MATH_SRC) $(MATH_DEMO)
	$(CC) $(CFLAGS) $^ -o $@

# Run canvas demo
run_canvas: $(CANVAS_OUT)
	./$(CANVAS_OUT)

# Run math demo
run_math: $(MATH_OUT)
	./$(MATH_OUT)

# Clean builds
clean:
	rm -f $(CANVAS_OUT) $(MATH_OUT)

