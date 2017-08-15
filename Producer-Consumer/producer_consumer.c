#include<stdio.h>
#include<stdlib.h>

int signal(int s);
int wait(int s);
void producer();
void consumer();
int main()
{
    int ch;
    printf("\nPRODUCER-CONSUMER PROBLEM");
    printf("\n---------------------------");
    printf("\nPress 1 For Producer");
    printf("\nPress 2 For Consumer");
    printf("\nPress 3 To Exit");
    printf("\nEnter Your Choice: ");
    scanf("%d",&ch);
    switch(ch)
    {
        case 1:
            break;
        case 2:
            break;
        case 3:
            exit(0);
            break;
    }

    
}