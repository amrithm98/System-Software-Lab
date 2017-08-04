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

int ready_queue[20][5];
//PID BURST WAIT Priority RemainingTime

int main()
{   
    int n,ch;

    printf("\nEnter Number of Processes: ");
    scanf("%d",&n);

    add_proc_to_queue(n);

    while(1){

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
    int quantum,remain=n,tim,count,flag;
    printf("\nEnter Time Quantum for RR: ");
    scanf("%d",quantum);
    printf("PID\tBurst-Time\tPriority\tWaiting-Time\tTurn-Around-Time\n");
    printf("----\t------------\t---------\t-------------\t---------------\n");
    for(tim=0,count=0;remain!=0;)
    {
        if(ready_queue[count][4]<=quantum && ready_queue[count][4]>0)
        {
            tim+=ready_queue[count][4];
            ready_queue[count][4]=0;   
            flag=1;
        }
        else if(ready_queue[count][4]>0){
            ready_queue[count][4]-=quantum;
            tim+=quantum;
        }
        if(ready_queue[count][4]==0 && flag==1)
        {
            remain--;
            printf("%d\t%d\t\t%d\t\t%d\t\t%d",ready_queue[count][0],ready_queue[count][1],ready_queue[count][3],ready_queue[count][2],ready_queue[count][1]+ready_queue[count][2]);
            flag=0;
        }
    }

}

void add_proc_to_queue(int n){
    int i,j;
    for(i=0;i<n;i++)
    {       
            printf("Enter Process PID:");
            scanf("%d",&ready_queue[i][0]);
            printf("Enter Process Burst Time:");
            scanf("%d",&ready_queue[i][1]);
            printf("Enter Process Priority( 0 for No Special Priority ):");
            scanf("%d",&ready_queue[i][3]);
            ready_queue[i][4]=ready_queue[i][1]; //Initital Remaining Time= Burst Time
    }
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
    for(i=0;i<n;i++)
    {
        for(j=0;j<n-i-1;j++)
        {
            if(ready_queue[i][by]>ready_queue[i+1][by])
            {
                swap(&ready_queue[i][0],&ready_queue[i+1][0]);
                swap(&ready_queue[i][1],&ready_queue[i+1][1]);
                swap(&ready_queue[i][2],&ready_queue[i+1][2]);
                swap(&ready_queue[i][3],&ready_queue[i+1][3]);
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