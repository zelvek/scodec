/*
   ============================================================================
   Name        : Scodec
   Author      : Pierre-Marie MOISAN, Hadrien BOUFFIER
   Version     : 1.0.0
   License     : MIT
   Description : Cipher and decipher any file using an order 4 square matrix
   ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

// Execution time
#include <time.h>
/*
    Not used anymore
   // open, read, write
 #include <sys/types.h>
 #include <sys/stat.h>
 #include <fcntl.h>
 */
#include "header/utility.h"
#include "header/cypher.h"
#include "header/conversion.h"
#include "header/config.h"

// Default max threads : 16
pthread_t threadsList[MAX_THREADS];

extern char *computedBuffer;

int main(int argc, char **argv)
{
        clock_t start = clock();
        // Default setting
        int threadsQty = 8;

        // STRUCT
        struct File iFile;
        ThreadArgs args;
        struct OutputFile oFile;

        struct Key key = {
                .size = 4,
                .lines = initMatrixLines(key.size)
        };

        // COUNTER
        int i = 0;
        int j = 0;
        int k = 0;
        long int bytesLeft = 0;

        // DATA
        int mode = 0;
        int keyLineLength = 0;
        float multiplier = 0;
        char buf[CHUNK_SIZE];
        int read = 0;
        int opt = 0;

        // ARG PARSING
        if( argc < 4 ) printf(USAGE_STR, argv[0]), emit(ERR_MORE_ARGS), exit(EXIT_FAILURE);

        while ( (opt = getopt(argc, argv, "k:f:cdt:")) != -1 ) {
                switch (opt) {
                case 'k':
                        strcpy(key.path, optarg);

                        if ( !(key.file = fopen(key.path, "r")) ) {
                                emit(ERR_NOFILE);
                                exit(EXIT_FAILURE);
                        }

                        if( !(fetchMatrix(key.lines, key.file, key.size) == 1) ) {
                                emit(ERR_MALFORMED_KEY);
                                exit(EXIT_FAILURE);
                        }

                        // We store it in a variable if we need to get KEY_LINE_LENGTH from argv later on
                        keyLineLength = KEY_LINE_LENGTH;

                        multiplier = ((BYTE/(float)key.size)*keyLineLength)/(float)BYTE;

                        if (getKeyPos(&key) != 1) {
                                emit(ERR_INVALID_KEY);
                                exit(EXIT_FAILURE);
                        }

                        break;
                case 'f':
                        strcpy(iFile.path, optarg);

                        iFile.file = fopen(iFile.path, "rb");

                        if (iFile.file < 0) {
                                emit(ERR_NOFILE);
                        }

                        iFile.length = filelen(iFile.file);
                        break;
                case 'c':
                        mode = FLG_CYPHER;

                        break;
                case 'd':
                        mode = FLG_DECYPHER;

                        break;
                case 't':
                        threadsQty = atoi(optarg);

                        break;
                default:
                        printf(USAGE_STR, argv[0]);
                        emit(ERR_MORE_ARGS);
                        exit(ERR_FAILURE);
                        break;
                }
        }

        switch (mode) {
        case FLG_CYPHER:
                strcpy(oFile.path, strcat(iFile.path, "c"));

                // Check if file already exists
                if (access(oFile.path, F_OK) != -1) {
                        // File already exists
                        unlink(oFile.path);
                }

                if ( !(oFile.file = fopen(oFile.path, "ab+")) ) {
                        emit(ERR_NO_RIGHTS);
                        exit(ERR_FAILURE);
                }

                bytesLeft = iFile.length;

                while(bytesLeft > 0) {
                        k++;
                        // Cypher loop
                        for (i = 0; i < threadsQty; i++) {
                                read = fread(&buf, CHUNK_SIZE, 1, iFile.file);
                                if ( (read != '\0' ||read == 0) ) {
                                        args.limit = (bytesLeft > CHUNK_SIZE) ? CHUNK_SIZE : bytesLeft;
                                        args.encryptedSize = args.limit*multiplier;
                                        args.keySize = key.size;
                                        memcpy(args.buffer, buf, CHUNK_SIZE);
                                        args.file = oFile.file;
                                        args.threadId = i;

                                        args.lines = initMatrixLines(key.size);
                                        args.lines = key.lines;

                                        //printf("__Creating thread %d...\n", i+1);

                                        pthread_create(&threadsList[i], NULL, &cypherChunk, &args);
                                        bytesLeft -= CHUNK_SIZE;

                                        //printf("__%ld left to encrypt\n", bytesLeft);

                                        if (bytesLeft < 0) {
                                                //printf("__bytesLeft below 0, exiting...\n");

                                                //printf("__Waiting for thread %d to exit before continuing...\n", i+1);
                                                pthread_join(threadsList[i], NULL);
                                                goto breakCypherLoop;
                                        }
                                        pthread_join(threadsList[i], NULL);

                                }else{
                                        pthread_join(threadsList[i], NULL);
                                        goto breakCypherLoop;
                                }
                        }
                        //printf("__Waiting for thread %d to exit before continuing...\n", i+1);

                        //pthread_join(threadsList[threadsQty-1], NULL);

                } // End of while

breakCypherLoop:
                fclose(iFile.file);
                fclose(oFile.file);

                break;
        case FLG_DECYPHER:
                strcpy(oFile.path, strcat(iFile.path, "d"));

                // Check if file already exists
                if (access(oFile.path, F_OK) != -1) {
                        // File already exists
                        unlink(oFile.path);
                }

                if ( !(oFile.file = fopen(oFile.path, "ab+")) ) {
                        emit(ERR_NO_RIGHTS);
                        exit(ERR_FAILURE);
                }

                bytesLeft = iFile.length;

                while(bytesLeft > 0) {
                        // Decypher loop
                        for (i = 0; i < threadsQty; i++) {
                                read = fread(&buf, CHUNK_SIZE, 1, iFile.file);
                                if ( (read != '\0' ||read == 0) ) {
                                        //if ( (fread(&buf, CHUNK_SIZE, 1, iFile.file)) ) {
                                        args.limit = (bytesLeft > CHUNK_SIZE) ? CHUNK_SIZE : bytesLeft;
                                        args.finalSize = args.limit/2;
                                        args.keySize = key.size;
                                        memcpy(args.buffer, buf, CHUNK_SIZE);
                                        args.file = oFile.file;
                                        args.threadsQty = threadsQty;
                                        args.threadId = i;

                                        for (j = 0; j < key.size; j++) {
                                                args.hybridIdPos[j] = key.hybridIdPos[j];
                                        }

                                        //printf("__Creating thread %d...\n", i+1);

                                        pthread_create(&threadsList[i], NULL, &decypherChunk, &args);

                                        bytesLeft -= CHUNK_SIZE;

                                        // printf("__%ld left to decrypt\n", bytesLeft);

                                        if (bytesLeft < 0) {
                                                //printf("__bytesLeft below 0, exiting...\n");

                                                //printf("__Waiting for thread %d to exit before continuing...\n", i+1);
                                                pthread_join(threadsList[i], NULL);
                                                goto breakDecypherLoop;
                                        }
                                }else{
                                        pthread_join(threadsList[i], NULL);
                                        goto breakDecypherLoop;
                                }
                                //printf("__Waiting for thread %d to exit before continuing...\n", i+1);
                                pthread_join(threadsList[i], NULL);
                        }
                }
breakDecypherLoop:
                fclose(iFile.file);
                fclose(oFile.file);

                break;

        } // End of switch

        clock_t finish = clock();
        double duration = (double)(finish-start)/CLOCKS_PER_SEC;

        printf("__Execution time : %2.4f", duration);

        // FREE key.lines && args.lines
        emit(SUCCESS);
        return 0;
}
