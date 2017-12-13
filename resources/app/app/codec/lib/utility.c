#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h> // fstat
#include <sys/stat.h> // fstat
#include <unistd.h> // fstat

#include "../header/utility.h"
#include "../header/config.h"
#include "../header/conversion.h"

int getKeyPos(struct Key *key) {
  int i = 0;
  int j = 0;
  int cnt = 0;
  int found = 0;
  int buffer = 0;
  int bit = 0;

  // Column by column
  for (i = 0; i < COL_NB; i++) {
    cnt = 0;
    // Row by row
    for (j = 0; j < key->size; j++) {
      // Store the most significant bit
      bit = (int)key->lines[j] << i;

      // if it's a "1", we increment our counter
      if ( (bit & 128) ) {
        cnt++;
        // We store j
        buffer = j;
      }
    }

      // If we found only one "1", it means that we found a key part
      if (cnt == 1) {

        // We then store key.hybridIdPos[line] = column
        key->hybridIdPos[buffer] = i+1;
        found++;
      }
  }
  if (found != key->size) {
    return ERR_INVALID_KEY;
  }

  return 1;
}
void printFile(FILE *file){
    char c;
    while ((c = getc(file)) != EOF) {
        printf("%c", c);
    }
}
void printBinFile(int fd, int size, int len){
    if(size > len){
        size = len;
    }

    unsigned char buf[size];
    int i = 0;
    int j = 0;
    int res = 0;
    int counter = 0;

    lseek(fd, 0, SEEK_SET);

    printf("Size => %d | Taille du fichier => %d octets\n", size, len);
    while ((res = read(fd, &buf, size)) != 0) {
        printf("Step %d | Valeur du read : %d\n", j, res);
        for (i = 0; i < size; i++) {
            printf("%d", buf[i%size]);
            counter++;
        }
        printf("\n");
        j++;
        lseek(fd, size, SEEK_CUR);
    }
    printf("Affichage de %d elements", counter);
}
void printMallocError(char* functionName){
  printf("ERR -- %s malloc went wrong\n", functionName);
  exit(EXIT_FAILURE);
}

void dbg() {
  printf(">> dgb <<\n");
}

char **initMatrixLines(int matrixSize){
    int i = 0;
    char **dest = NULL;

    if( !(dest = malloc(sizeof(unsigned char*)*matrixSize)) ) {
        emit(ERR_MALLOC);
    }else{
      for (i = 0; i < matrixSize; i++) {
        if( !(dest[i] = malloc(sizeof(unsigned char))) ) {
            emit(ERR_MALLOC);
        }
      }
    }

    return dest;
}

long int filelen(FILE* fp){
  fseek(fp, 0, SEEK_END);

  long int length = ftell(fp);

  rewind(fp);

  return length;
}

int cleanup(int iFile, FILE* oFile, FILE* keyFile){
    int i = 0;

    if(iFile != -1){
       close(iFile);
       i++;
    }

    if(keyFile != NULL){
        fclose(keyFile);
        i++;
    }

    if(oFile != NULL){
        fclose(oFile);
        i++;
    }

    return i;
}
void emit(int errid){
  char *errstr;

  switch (errid) {
    case ERR_FAILURE:
      errstr = "Failure";
      break;
    case ERR_SUCCESS:
      errstr = "Success";
      break;
    case ERR_MORE_ARGS:
      errstr = "Not enough arguments supplied";
      break;
    case ERR_NOFILE:
      errstr = "Cannot open the input file";
      break;
    case ERR_MALFORMED_KEY:
      errstr = "Key format is not recognized";
      break;
    case ERR_NO_RIGHTS:
      errstr = "Can't create new file";
      break;
    case ERR_MALLOC:
      errstr = "Malloc failed";
      break;
    case ERR_INVALID_KEY:
      errstr = "The key is well formatted but invalid";
      break;
    default:
      errstr = "Unrecognized error";
      break;
  }
    printf("__%s__%d", errstr, errid);
}
