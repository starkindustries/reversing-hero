all: build run

filename = x4_source

flags = -g # used for gdb

build: $(filename).c
	gcc $(flags) $(filename).c -o $(filename).o
	# *********************
	# Build Successful
	# *********************

run:
	# *********************
	# Running Program
	# *********************
	./$(filename).o
	# *********************
	# Run Completed
	# *********************