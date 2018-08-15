#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include "numericalBaseConversionsFuncs.h"

int main(int argc, char **argv) {
	int c;
	char *base1;
	char *base2;
	int i = 0;
	while ((c = getopt(argc, argv, "io:")) != -1)
		switch (c){
			case 'i':
				base1 = argv[optind];
				optind++;
				printf("%c", opterr);
				break;
			case 'o':
				base2 = argv[optind -1];
				for(i = optind; i <= argc-1; i++){
					fromAnyBaseToDecimal(argv[i], base1, base2);
				}
				break;
			case '?':
				if (optopt == 'o'){
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);	
				}else if (optopt == 'i'){
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
				}else if (isprint (optopt)){
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				}
				else{
					fprintf (stderr,"Unknown option character `\\x%x'.\n", optopt);
				}
				break;
			default:
				fprintf (stderr, "Error, the input is invalid. `\\x%x'.\n", optopt);
			}
		return 0;
}