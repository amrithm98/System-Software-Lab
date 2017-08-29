#include<stdio.h>
#include<stdlib.h>

void Signal(int *s);
void wait(int *s);
void producer();
void consumer();

int mutex=1,empty=0,full=0,item=1;

void wait(int *s)
{
    if(*s<=0)
    {
        *s=0;
    }
    else
        (*s)--;
}
 
void Signal(int *s)
{
    (*s)++;
}

void producer()
{
    wait(&empty);    
    wait(&mutex);
    printf("\nProducer Produced Item: %d",item++);
    Signal(&mutex);
    Signal(&full);
    // printf("\n%d %d %d",mutex,empty,full);
}

void consumer()
{
    wait(&full);
    wait(&mutex);
    printf("\nConsumer Consumed Item: %d",--item);
    Signal(&mutex);
    Signal(&empty);
    // printf("\n%d %d %d",mutex,empty,full);
}

int main()
{
    int ch,n;
    printf("\nEnter The Buffer Size: ");
    scanf("%d",&n);
    empty=n;
    full=0;
    mutex=1;
    while(1)
    {
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
                if(mutex==1 && empty!=0)
                    producer();
                else
                    printf("\nBuffer Full!!");
                break;
            case 2:
                if(mutex==1 && full!=0)
                    consumer();
                else
                printf("\nBuffer Empty!!");
                break;
            case 3:
                exit(0);
                break;
        }
    }
    

    
}