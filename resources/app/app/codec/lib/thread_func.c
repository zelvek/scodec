#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "../header/config.h"
#include "../header/thread_func.h"

ThreadArgs *initCypherThreadArgs(int limit, int encryptedSize, int keySize, char buffer[CHUNK_SIZE]){
    ThreadArgs *args;

    args = malloc(sizeof(*args));

    args->limit = limit;
    args->encryptedSize = encryptedSize;
    args->keySize = keySize;
    strncpy(args->buffer, buffer, CHUNK_SIZE);

    return args;
}
