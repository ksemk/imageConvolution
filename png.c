#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

unsigned long long timestamp();         // Funkcja zwracająca aktualny licznik cykli procesora
double cpu_frequency = 2.5e9;

#define ERROR                                                   \
	fprintf (stderr, "ERROR at %s:%d.\n", __FILE__, __LINE__) ;   \
	return -1 ;                                                   \


void filter(unsigned char *M, unsigned char *W, int width, int height) { 
    int kernel[3][3] = { 
        {-1, -1, 0}, 
        {-1, 0, 1}, 
        {0, 1, 1} 
    }; 
 
    for (int y = 1; y < height - 1; y++) { 
        // Wskaźniki do odpowiednich wierszy 
        unsigned char *row_prev = M + (y - 1) * width; 
        unsigned char *row_curr = M + y * width; 
        unsigned char *row_next = M + (y + 1) * width; 
        unsigned char *row_out = W + y * width; 
 
        for (int x = 1; x < width - 1; x++) { 
            int sum = 0; 
 
            // Operacje na elementach tablicy poprzez wskaźniki 
            sum += row_prev[x - 1] * kernel[0][0]; 
            sum += row_prev[x] * kernel[0][1]; 
            sum += row_prev[x + 1] * kernel[0][2]; 
            sum += row_curr[x - 1] * kernel[1][0]; 
            sum += row_curr[x] * kernel[1][1]; 
            sum += row_curr[x + 1] * kernel[1][2]; 
            sum += row_next[x - 1] * kernel[2][0]; 
            sum += row_next[x] * kernel[2][1]; 
            sum += row_next[x + 1] * kernel[2][2]; 
 
            // Normalizacja wyniku 
            row_out[x] = (sum + 765) / 8; 
        } 
    } 
}

void measure_performance(unsigned char *M, unsigned char *W, int width, int height, double cpu_frequency, int bit_depth) {
    unsigned long long tstamp1, tstamp2;
    double elapsed_time, speed_GB, speed_Mpx;
    unsigned int pixel_num;

    tstamp1 = timestamp();
    filter(M, W, width, height);
    tstamp2 = timestamp();
    unsigned long long cycles = tstamp2 - tstamp1;

    elapsed_time = (double)cycles / cpu_frequency;
    pixel_num = width * height;
    speed_GB = (pixel_num * bit_depth / 8) / elapsed_time / 1e9;
    speed_Mpx = pixel_num / elapsed_time / 1e6;

    printf("Cycles: %llu\n", cycles);
    printf("Czas wykonania: %f sekund\n", elapsed_time);
    printf("Przepustowość pamięci: %f GB/s\n", speed_GB);
    printf("Szybkość renderowania: %f Mpx/s\n", speed_Mpx);
}

int main (int argc, char ** argv)
{
	if (2 != argc)
	{
		printf ("\nUsage:\n\n%s file_name.png\n\n", argv[0]) ;

		return 0 ;
	}

	const char * file_name = argv [1] ;
	
	#define HEADER_SIZE (1)
	unsigned char header [HEADER_SIZE] ;

	FILE *fp = fopen (file_name, "rb");
	if (NULL == fp)
	{
		fprintf (stderr, "Can not open file \"%s\".\n", file_name) ;
		ERROR
	}

	if (fread (header, 1, HEADER_SIZE, fp) != HEADER_SIZE)
	{
		ERROR
	}

	if (0 != png_sig_cmp (header, 0, HEADER_SIZE))
	{
		ERROR
	}

	png_structp png_ptr = 
		png_create_read_struct
			(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );
	if (NULL == png_ptr)
	{
		ERROR
	}

	png_infop info_ptr = png_create_info_struct (png_ptr);
	if (NULL == info_ptr)
	{
		png_destroy_read_struct (& png_ptr, NULL, NULL);

		ERROR
	}

	if (setjmp (png_jmpbuf (png_ptr))) 
	{
		png_destroy_read_struct (& png_ptr, & info_ptr, NULL);

		ERROR
	}

	png_init_io       (png_ptr, fp);
	png_set_sig_bytes (png_ptr, HEADER_SIZE);
	png_read_info     (png_ptr, info_ptr);

	png_uint_32  width, height;
	int  bit_depth, color_type;
	
	png_get_IHDR
	(
		png_ptr, info_ptr, 
		& width, & height, & bit_depth, & color_type,
		NULL, NULL, NULL
	);

	if (8 != bit_depth)
	{
		ERROR
	}
	if (0 != color_type)
	{
		ERROR
	}

	size_t size = width ;
	size *= height ;

	unsigned char * M = malloc (size) ;

	png_bytep ps [height] ;
	ps [0] = M ;
	for (unsigned i = 1 ; i < height ; i++)
	{
		ps [i] = ps [i-1] + width ;
	}
	png_set_rows (png_ptr, info_ptr, ps);
	png_read_image (png_ptr, ps) ;

	printf 
	(
		"Image %s loaded:\n"
		"\twidth      = %u\n"
		"\theight     = %u\n"
		"\tbit_depth  = %u\n"
		"\tcolor_type = %u\n"
		, file_name, width, height, bit_depth, color_type
	) ;

	unsigned char * W = malloc (size) ;

	measure_performance(M, W, width, height, cpu_frequency, bit_depth);



	png_structp write_png_ptr =
		png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (NULL == write_png_ptr)
	{
		ERROR
	}

	for (unsigned i = 0 ; i < height ; i++)
	{
		ps [i] += W - M ;
	}
	png_set_rows (write_png_ptr, info_ptr, ps);

	FILE *fwp = fopen ("out.png", "wb");
	if (NULL == fwp)
	{
		ERROR
	}

	png_init_io   (write_png_ptr, fwp);
	png_write_png (write_png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
	fclose (fwp);

	return 0;
}
