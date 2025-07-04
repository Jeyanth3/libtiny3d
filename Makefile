# Compiler and flags
CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -std=c99
LDFLAGS = -lm

# Source files
CANVAS_SRC = src/canvas.c
MATH_SRC = src/math3d.c
RENDER_SRC = src/renderer.c

# Demos
CANVAS_DEMO = demo/main.c
MATH_DEMO = tests/test_math.c
RENDER_DEMO = demo/render_main.c

# Executables
CANVAS_OUT = canvas_demo
MATH_OUT = test_math
RENDER_OUT = render_demo

# Phony targets
.PHONY: all clean run_canvas run_math run_render

# Default target
all: $(CANVAS_OUT) $(MATH_OUT) $(RENDER_OUT)

# Build canvas demo
$(CANVAS_OUT): $(CANVAS_SRC) $(CANVAS_DEMO)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Build math demo
$(MATH_OUT): $(MATH_SRC) $(MATH_DEMO)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Build renderer demo
$(RENDER_OUT): $(CANVAS_SRC) $(MATH_SRC) $(RENDER_SRC) $(RENDER_DEMO)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Run canvas demo
run_canvas: $(CANVAS_OUT)
	./$(CANVAS_OUT)

# Run math demo
run_math: $(MATH_OUT)
	./$(MATH_OUT)

# Run render demo
run_render: $(RENDER_OUT)
	./$(RENDER_OUT)

# Clean build files
clean:
	rm -f $(CANVAS_OUT) $(MATH_OUT) $(RENDER_OUT)
