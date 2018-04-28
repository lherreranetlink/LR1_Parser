#include "fileutils.h"

char* getNextLine(FILE* fd)
{
    static int filepos = 0;
    int c;
    char buff[BUFSIZ], *buffp, *line;

    fseek(fd, filepos, SEEK_SET);

    //Remove Spaces
    while(isspace(c = fgetc(fd)))
        filepos++;

    for (buffp = buff; c != '\n' && c != EOF; c = fgetc(fd), filepos++)
        *buffp++ = c;

    filepos++;
    *buffp = '\0';
    line = (char*) malloc(strlen(buff));
    strcpy(line, buff);

    return line;
}
