#include<stdio.h>
#include<stdlib.h>

int Signal(int chopSticks[],int i);
int wait(int chopSticks[],int i);
void eat(int chopSticks[],int i,int n);
void think(int chopSticks[],int i,int n);

void eat(int chopSticks[],int i,int n)
{
    int lock;
    lock=wait(chopSticks,i);

    if(lock==0)
        return;

    else
    {
        printf("\nPhilosopher %d Starts to Eat",i+1);
        chopSticks[i]=0;
        chopSticks[(i+1)%n]=0;
    }
}

void think(int chopSticks[],int i,int n)
{
    int lock;
    lock=Signal(chopSticks,i);

    if(lock==0)
        return;
    else
    {
        printf("\nPhilosopher %d Starts to Think",i+1);
        chopSticks[i]=1;
        chopSticks[(i+1)%n]=1;
    }

}


int wait(int chopSticks[],int i)
{
    if(chopSticks[i]==0)
    {
        printf("\nNo ChopStick Available");
        return 0;
    }
    else    
        return 1;
}
 
int Signal(int chopSticks[],int i)
{
    if(chopSticks[i]==1)
    {
        printf("\nChopStick Available");
        return 0;
    }
    else    
        return 1;
}

int main()
{
    int ch,n,i;

    printf("\nEnter No of Philosophers: ");
    scanf("%d",&n);

    int chopSticks[n];

    for(i=0;i<n;i++)
        chopSticks[i]=1; //Initially all will have 1 chopstick each

    while(1)
    {
        printf("\nDining-Philosopher PROBLEM");
        printf("\n---------------------------");
        printf("\nPress 1 To Eat");
        printf("\nPress 2 To Think");
        printf("\nPress 3 To Exit");
        printf("\nEnter Your Choice: ");
        scanf("%d",&ch);
        switch(ch)
        {
            case 1:
                printf("\nWhich Philosopher wants to eat ? :");
                scanf("%d",&i);
                eat(chopSticks,i-1,n);
                break;
            case 2:
                printf("\nWhich Philosopher wants to think?");
                scanf("%d",&i);
                think(chopSticks,i-1,n);
                break;
            case 3:
                exit(0);
            default:
                break;
        }
    }
    
}