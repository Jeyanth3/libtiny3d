# Compiler and flags
CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -std=c99
LDFLAGS = -lm

# Source files
CANVAS_SRC = src/canvas.c
MATH_SRC = src/math3d.c
RENDER_SRC = src/renderer.c
SOCCERBALL_SRC = src/soccerball.c

# Demos and tests
MAIN_DEMO = demo/main.c
MATH_DEMO = tests/test_math.c
RENDER_DEMO = demo/render_main.c

# Executables
MAIN_OUT = main_demo
MATH_OUT = test_math
RENDER_OUT = render_demo

# Phony targets
.PHONY: all clean run_main run_math run_render frames

# Default target
all: $(MAIN_OUT) $(MATH_OUT) $(RENDER_OUT)

# Build final soccer ball animation generator
$(MAIN_OUT): $(CANVAS_SRC) $(MATH_SRC) $(RENDER_SRC) $(SOCCERBALL_SRC) $(MAIN_DEMO)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Build math test
$(MATH_OUT): $(MATH_SRC) $(MATH_DEMO)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Build renderer demo (if used)
$(RENDER_OUT): $(CANVAS_SRC) $(MATH_SRC) $(RENDER_SRC) $(RENDER_DEMO)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Run animation generator
run_main: $(MAIN_OUT)
	./$(MAIN_OUT)

# Shortcut to generate animation frames
frames: run_main

# Run math test
run_math: $(MATH_OUT)
	./$(MATH_OUT)

# Run renderer demo
run_render: $(RENDER_OUT)
	./$(RENDER_OUT)

# Clean
clean:
	rm -f $(MAIN_OUT) $(MATH_OUT) $(RENDER_OUT) *.pgm
