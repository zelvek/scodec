#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "../header/utility.h"
#include "../header/conversion.h"
#include "../header/config.h"

int fetchMatrix(char **matrixDest, FILE* keyFile, const int matrixSize){
    int i = 0;
    int counter = 0;
    char **bufMatrix = NULL;

    if( !(bufMatrix = malloc(sizeof(char*)*matrixSize)) ) {
        printf("Fetch matrix malloc failed\n");
        exit(ERR_MALLOC);
    }else{
      for (i = 0; i < matrixSize; i++) {
        if( !(bufMatrix[i] = malloc(sizeof(char)*8)) ) {
            printf("Fetch matrix malloc failed\n");
            exit(ERR_MALLOC);
        }
      }
    }

    /* BUG
    // Returns nothing when using a string as format ?
    char *format = NULL;

    format = malloc(sizeof(char)*100);

    sprintf(format, "G%dC=[", matrixSize);

    for (i = 0; i < matrixSize; i++) {
        strcat(format, " %%s");
    }

    // We could add strcat(format, "]") but since it is not necessary, we won't spend time for it

    counter = fscanf(keyFile, format, matrixDest[0], matrixDest[1], matrixDest[2], matrixDest[3]);*/

    // Size of 4 is hardcoded here
    counter = fscanf(keyFile, "G4C=[%s %s %s %s]", bufMatrix[0], bufMatrix[1], bufMatrix[2], bufMatrix[3]);

    if (counter != matrixSize) {
        return -1;
    }

    for (i = 0; i < 4; i++) {
        matrixDest[i] = strtol(bufMatrix[i], NULL, 2);
    }

    return 1;
}
