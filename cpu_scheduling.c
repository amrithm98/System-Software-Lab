#include<stdio.h>
#include<stdlib.h>

void sjf_scheduling(int n);
void fcfs_scheduling(int n);
void priority_scheduling(int n);
void round_robin_scheduling(int n);
void add_proc_to_queue(int);
void display_ready_queue(int n);
void sort_ready_queue(int n,int by);//Sort By Burst or Priority
void swap(int *x,int *y);
void copy_queue(int ready_queue[][5],int n);

int ready_queue[20][5];
int input_queue[20][5];
//PID BURST WAIT Priority RemainingTime

int main()
{   
    int n,ch;

    printf("\nEnter Number of Processes: ");
    scanf("%d",&n);

    add_proc_to_queue(n);

    while(1){
        copy_queue(ready_queue,n);
        printf("\nPress 1 To Implement SJF Scheduling");
        printf("\nPress 2 To Implement FCFS Scheduling");
        printf("\nPress 3 To Implement Priority Scheduling");
        printf("\nPress 4 To Implement Round Robin Scheduling");
        printf("\nPress 5 To Exit");
        printf("\nEnter Your Choice: ");
        scanf("%d",&ch);

        switch(ch)
        {
    
            case 1:
                sjf_scheduling(n);
                break;  
            case 2:
                fcfs_scheduling(n);
                break;
            case 3:
                priority_scheduling(n);
                break;
            case 4:
                round_robin_scheduling(n);
                break;
            case 5:
                exit(0);
                break;
            default:
                break;
        }
    }
}

void copy_queue(int ready_queue[][5],int n)
{
    int i,j;
    for(i=0;i<n;i++)
    {
        for(j=0;j<5;j++)
        {
            ready_queue[i][j]=input_queue[i][j];
        }
    }
}

void sjf_scheduling(int n)
{
    sort_ready_queue(n,1);
    ready_queue[0][2]=0; //Waiting Time of First Process
    int i;
    //Calculate Waiting Time
    for(int i=1;i<n;i++)
    {
        ready_queue[i][2]=ready_queue[i-1][2]+ready_queue[i-1][1];
    }
    printf("\n SJF SCHEDULING : \n---------------------\n");
    display_ready_queue(n);
}

void fcfs_scheduling(int n)
{
    ready_queue[0][2]=0; //Waiting Time of First Process
    int i;
    //Calculate Waiting Time
    for(int i=1;i<n;i++)
    {
        ready_queue[i][2]=ready_queue[i-1][2]+ready_queue[i-1][1];
    }
    printf("\n FCFS SCHEDULING : \n---------------------\n");
    display_ready_queue(n);
}

void priority_scheduling(int n)
{
    sort_ready_queue(n,3); //Sort By Priority
    ready_queue[0][2]=0; //Waiting Time of First Process
    int i;
    //Calculate Waiting Time
    for(int i=1;i<n;i++)
    {
        ready_queue[i][2]=ready_queue[i-1][2]+ready_queue[i-1][1];
    }
    printf("\n Priority SCHEDULING : \n---------------------\n");
    display_ready_queue(n);
}

void round_robin_scheduling(int n)
{
    int quantum,remain=n,tim=0,count,flag=1,i;
    printf("\nEnter Time Quantum for RR: ");
    scanf("%d",&quantum);
    printf("\nRound Robin SCHEDULING : \n---------------------\n");

    while(1){

        flag=1;
        
        for(i=0;i<n;i++)
        {
            if(ready_queue[i][4]>0)
            {
                flag=0;
                if(ready_queue[i][4]>quantum)
                {
                    tim+=quantum;
                    ready_queue[i][4]-=quantum;
                }
                else
                {
                    tim+=ready_queue[i][4];
                    ready_queue[i][2]=tim-ready_queue[i][1];
                    ready_queue[i][4]=0;
                }
            }
        }
        if(flag)
        {
            break;
        }
    }
    display_ready_queue(n);

}

void add_proc_to_queue(int n){
    int i,j;
    for(i=0;i<n;i++)
    {       
            printf("Enter Process PID:");
            scanf("%d",&input_queue[i][0]);
            printf("Enter Process Burst Time:");
            scanf("%d",&input_queue[i][1]);
            printf("Enter Process Priority( 0 for No Special Priority ):");
            scanf("%d",&input_queue[i][3]);
    }
    for(i=0;i<n;i++)
    {
        //Initital Remaining Time= Burst Time
        input_queue[i][4]=input_queue[i][1];
    }
    copy_queue(ready_queue,n);
    printf("\nINPUT PROCESSES:\n");
    printf("---------------------\n");
    display_ready_queue(n);
}

void display_ready_queue(int n){

    int i,j;
    printf("PID\tBurst-Time\tPriority\tWaiting-Time\tTurn-Around-Time\n");
    printf("----\t------------\t---------\t-------------\t---------------\n");
    for(i=0;i<n;i++)
    {
            printf("%d\t%d\t\t%d\t\t%d\t\t%d",ready_queue[i][0],ready_queue[i][1],ready_queue[i][3],ready_queue[i][2],ready_queue[i][1]+ready_queue[i][2]);
            printf("\n");
    }
}

void sort_ready_queue(int n,int by)
{
    int i,j;
    for(i=0;i<n-1;i++)
    {
        for(j=0;j<n-1;j++)
        {
            if(ready_queue[j][by]>ready_queue[j+1][by])
            {
                printf("\nBefore %d\t%d",ready_queue[j][1],ready_queue[j+1][1]);
                swap(&ready_queue[j][0],&ready_queue[j+1][0]);
                swap(&ready_queue[j][1],&ready_queue[j+1][1]);
                swap(&ready_queue[j][2],&ready_queue[j+1][2]);
                swap(&ready_queue[j][3],&ready_queue[j+1][3]);
                swap(&ready_queue[j][4],&ready_queue[j+1][4]);
                printf("\nAfter %d\t%d",ready_queue[j][1],ready_queue[j+1][1]);
            }
        }
    }
}

void swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}