all: build

source = x1_source.c
outfile = x1_source.o

# *********************
# Ignored Warnings
# *********************
# warning: passing argument 1 of ‘printf’ from incompatible pointer type [-Wincompatible-pointer-types]
# warning: format not a string literal and no format arguments [-Wformat-security]
# printf(&question_mark);
ignore_warnings = -Wno-incompatible-pointer-types \
	-Wno-format-security

build: $(source)
	gcc $(source) $(ignore_warnings) -o $(outfile)
	# *********************
	# Build Successful
	# *********************

run:
	./$(outfile)