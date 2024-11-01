# Główny cel, który buduje wszystkie zależności: fibb oraz timestamp32.s
all: program

# Cel program, który kompiluje kod w C, asemblerze oraz skrypt timestamp32.s do pliku wykonywalnego program
program: png.c timestamp64.s
	gcc -m64 png.c timestamp64.s -lpng -o program

# Cel clean, który usuwa skompilowane pliki wykonywalne i pliki obiektowe
clean:
	rm -f program *.o