#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *fptr;
    char fileName[20];
    char line[200];

    printf("\nEnter Source File Name: ");
    scanf("%s",fileName);

    fptr=fopen(fileName,"r");

    if(fptr==NULL)
    {
        printf("\nCould Not Open File!!");
        exit(0);
    }

    while(fgets(line,sizeof(line),fptr))
    {
        printf("%s",line);    
    }

    fclose(fptr);
}