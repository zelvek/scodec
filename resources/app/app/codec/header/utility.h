#ifndef CONVERSION_H_INCLUDED
#define CONVERSION_H_INCLUDED

//int getKeyPos(struct Key *key);
void printFile(FILE *file);
void printMallocError(char* functionName);
long int filelen(FILE* fp);
void dbg();
char **initMatrixLines(int matrixSize);
void printBinFile(int fd, int size, int len);
int cleanup(int iFile, FILE* oFile, FILE* keyFile);
void *smalloc(long n);
void emit(int errno);

#endif // CONVERSION_H_INCLUDED
