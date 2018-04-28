#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

int main()
{
    FILE* file;
    if ((file  = fopen("example", "r")) == NULL)
    {
        fprintf(stderr, "file to parse does not exists\n");
        exit(1);
    }
    parse(file);
    return 0;
}
