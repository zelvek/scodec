#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <math.h>

#include "../header/config.h"
#include "../header/cypher.h"
#include "../header/utility.h"
#include "../header/conversion.h"

extern pthread_t threadsList[MAX_THREADS];

void *cypherChunk(void *_args){
    ThreadArgs *args = (ThreadArgs *)_args;
    //printf("\n%d %s %f %d %d\n", args->limit, args->buffer, args->encryptedSize, args->keySize, args->threadId);

    int i = 0;
    int j = 0;
    int k = 0;
    char bit = 0;
    char *computedBuffer = NULL;
    char res = 0;

    computedBuffer = malloc(sizeof(char)*args->encryptedSize);
    if (computedBuffer == NULL) {
        emit(ERR_MALLOC);
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < args->limit; i++) {
        //printf("\nbuf[%d] = %c\n", i, args->buffer[i]);

        for (j = BYTE/args->keySize; j > 0; j--) {
            res = 0;

            for(k = args->keySize; k > 0; k--) {
                bit = args->buffer[i] >> ( (k-1)+(args->keySize*(j-1)) );
                //printf("\nbit %02X, buf[i] %02X", bit, args->buffer[i]);

                if (bit & 1) {
                    res = res^(long int)args->lines[args->keySize-k];
                    //printf("\n>> %d <<", args->lines[args->keySize-k]);
                }
            }

            // Pour i = 0, j = 2 puis j = 1...
            // Nous voulons 0 puis 1...
            // Donc i+i+((BYTE/args->keySize)-j)
            // 0+0+((8/4)-2) = 0
            // 0+0+((8/4)-1) = 1
            // 1+1+((8/4)-2) = 2
            // 1+1+((8/4)-1) = 3
            computedBuffer[i+i+((BYTE/args->keySize)-j)] = res;
            //printf("\ncomputedBuffer[%d+%d+((%d/%d)-%d) = %d] = %c\n", i, i, BYTE, args->keySize, j, i+i+((BYTE/args->keySize)-j), computedBuffer[i+i+((BYTE/args->keySize)-j)]);
        }
    }

    if (args->threadId != 0) {
        pthread_join(threadsList[args->threadId], NULL);
    }

    //printf("%s\n", "__dans cypher après premier join");
    //printf("%ld\n", args->limit);
    fwrite(computedBuffer, 1, args->encryptedSize, args->file);

    free(computedBuffer);
    computedBuffer = NULL;

    pthread_exit(NULL);
}

void *decypherChunk(void *_args){
    ThreadArgs *args = (ThreadArgs *)_args;

    int i = 0;
    int j = 0;
    char *computedBuffer = NULL;
    int bufi = 0;

    computedBuffer = malloc(sizeof(char)*args->limit);

    for (i = 0; i < args->limit; i+=2) {
      bufi = i/2;
        //printf("\nbuf[%d] = %c\n", i, args->buffer[i]);
        computedBuffer[bufi] = 0;

        for (j = 0; j < args->keySize; j++) {
            //printf("%d\n", args->hybridIdPos[j]);
            computedBuffer[bufi] |= ((( args->buffer[i] >> (BYTE - args->hybridIdPos[j]) ) & 1) << (args->keySize - j + 3));
            computedBuffer[bufi] |= ((( args->buffer[i+1] >> (BYTE - args->hybridIdPos[j]) ) & 1) << (args->keySize - j - 1));
        }
    }

    if (args->threadId != 0) {
        pthread_join(threadsList[args->threadId], NULL);
    }

    //printf("__Encrypted size %lf\n", args->finalSize);
    fwrite(computedBuffer, args->finalSize, 1, args->file);

    free(computedBuffer);
    computedBuffer = NULL;

    pthread_exit(NULL);
}
