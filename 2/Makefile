all: build run

filename = x2_source

# *********************
# Ignored Warnings
# *********************
# warning: passing argument 1 of ‘printf’ from incompatible pointer type [-Wincompatible-pointer-types]
# warning: format not a string literal and no format arguments [-Wformat-security]
# printf(&question_mark);
ignore_warnings = -Wno-incompatible-pointer-types \
	-Wno-format-security

flags = -g # used for gdb

build: $(filename).c
	gcc $(flags) $(filename).c $(ignore_warnings) -o $(filename).o
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