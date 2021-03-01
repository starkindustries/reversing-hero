all: build run

main = x6_source

# Flags:
# -g		for GDB
# -Wall		show all warnings
flags = -g -Wall

sources = ../utils/helper.c

includes = -I../utils

build: $(main).c
	gcc $(flags) $(main).c $(sources) $(includes) -o $(main).o
	# *********************
	# Build Successful
	# *********************

run:
	# *********************
	# Running Program
	# *********************
	./$(main).o
	# *********************
	# Run Completed
	# *********************