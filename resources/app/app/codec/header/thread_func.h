#ifndef THREAD_FUNC_H_INCLUDED
#define THREAD_FUNC_H_INCLUDED

ThreadArgs *initCypherThreadArgs(int limit, int encryptedSize, int keySize, char buffer[CHUNK_SIZE]);

#endif // THREAD_FUNC_H_INCLUDED
