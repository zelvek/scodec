#ifndef PARAMS_H_INCLUDED
#define PARAMS_H_INCLUDED

// DEFINE
#define BYTE 8
//#define CHUNK_SIZE 131072 //131Ko
#define CHUNK_SIZE 524288 //524ko
//#define CHUNK_SIZE 2097152 //2Mo
#define USAGE_STR "Usage : %s -k <key path> -f <file path> [-cd]\n"
#define KEY_LINE_LENGTH 8
#define SUCCESS 1
#define COL_NB 8
#define MAX_THREADS 16

// STRUCT
struct Key{
    FILE *file;
    char path[255];
    char **lines;
    char computed;
    int size;
    int hybridIdPos[4];
};

struct File{
    FILE *file;
    char path[255];
    long int length;
};

struct OutputFile{
    FILE *file;
    char path[255];
    long int finalSize;
};

typedef struct{
  long int limit;
  int keySize;
  int threadsQty;
  long int encryptedSize;
  char **lines;
  FILE *file;
  char buffer[CHUNK_SIZE];
  int threadId;
  double finalSize;
  int hybridIdPos[4];
  float multiplier;
} ThreadArgs;

// ENUM
enum flags{
  FLG_CYPHER,
  FLG_DECYPHER
};

enum errors{
  ERR_FAILURE, // 0
  ERR_SUCCESS, // 1
  ERR_MALLOC, // ...
  ERR_NOFILE,
  ERR_NO_RIGHTS,
  ERR_MALFORMED_KEY,
  ERR_MORE_ARGS,
  ERR_INVALID_KEY
};

#endif // PARAMS_H_INCLUDED
