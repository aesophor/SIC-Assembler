#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** args)
{
    char data[64] = "C\'EOF\'";

    char* token = strtok(data, "\'"); 
    for(int i=0; token; i++)
    {
        printf("round %d: %s\n", i, token);
        token = strtok(NULL, "\'");
    }
    printf("%s\n", token);
    return 0;
}
