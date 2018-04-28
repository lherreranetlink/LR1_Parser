#include "stringutils.h"

char** split(char* str, char delimiter)
{
    int count, index;
    char *strp, symbol[BUFSIZ], *symbolp;

    for (strp = str, count = 0; *strp != '\0'; strp++)
        if (*strp == delimiter)
            count++;

    char** strSplit = (char**) malloc(sizeof(char*) * ++count);

    for (index = 0, symbolp = symbol, strp = str; *strp != '\0'; strp++)
    {
        *symbolp++ = *strp;
        if (*strp == delimiter)
        {
            *--symbolp = '\0';
            strSplit[index] = (char*) malloc(strlen(symbol));
            strcpy(strSplit[index++], symbol);
            symbolp = symbol;
        }
    }
    *symbolp = '\0';
    strSplit[index] = (char*) malloc(strlen(symbol));
    strcpy(strSplit[index], symbol);
    return strSplit;
}

int integerArrayLength(int* arr)
{
    return (int) (sizeof(arr) / sizeof(arr[0]));
}
