#ifndef CONVERSION_H_INCLUDED
#define CONVERSION_H_INCLUDED

char* int_to_bin(int number, int elementLength);
int parse_text_to_bin(char ***dest, char *text, int matrixSize, int elementLength);
char ***init_parsed_bin(char *text, int matrixSize, int elementLength);
int check_padding(int n, int matrixSize);
char **generate_identity(int matrixSize);
char **generate_padding(int matrixSize);
char **combine_array(char** idMatrix, char ** paddingMatrix, int matrixSize);
int count_str(char* str, char *str2);

#endif // CONVERSION_H_INCLUDED
