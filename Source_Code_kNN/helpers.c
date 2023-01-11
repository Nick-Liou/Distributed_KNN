
#include "helpers.h"

void debug(int rank, char *format, ...) {

	va_list args; /* Variable argument list */
	/* Initialize variable argument list;
	`format is last argument before 
	*/
	va_start(args, format);
	//printf("%12.6f1 %2d", get_timer(), rank);
	printf("Rank : %2d |", rank);
	vprintf (format, args);
	
	/* Clean up variable argument list */ 
	va_end (args);
}


