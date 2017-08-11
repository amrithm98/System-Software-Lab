#include<stdio.h>
#include<stdlib.h>

void copy_queue(int tracks_copy[],int n);
void fcfs_sched(int n,int head);
void scan_sched(int n,int head);
void cscan_sched(int n,int head);
void sort(int n);
int tracks[20],tracks_copy[20];

int main()
{
    int head=0,n,ch,i;
    while(1)
    {
        printf("\nDISK SCHEDULING METHODS");
        printf("\n-------------------------");
        printf("\nPress 1 To Enter Head Positions");
        printf("\nPress 2 To Do FCFS Scheduling");
        printf("\nPress 3 To Do SCAN Scheduling");
        printf("\nPress 4 To Do C-SCAN Scheduling");
        printf("\nPress 5 To Exit");
        printf("\nEnter Your Choice:");
        scanf("%d",&ch);
        switch(ch)
        {
            case 1:
                printf("\nEnter Number of Positions(Queue Size):");
                scanf("%d",&n);
                printf("\nEnter Current Head Position:");
                scanf("%d",&head);
                printf("\nEnter %d positions:",n);
                for(i=0;i<n;i++)
                {
                    scanf("%d",&tracks[i]);
                }
                break;
            case 2:
                copy_queue(tracks_copy,n);
                fcfs_sched(n,head);
                break;
            case 3:
                copy_queue(tracks_copy,n);   
                scan_sched(n,head);     
                break;
            case 4:
                copy_queue(tracks_copy,n);
                cscan_sched(n,head);
                break;
            case 5:
                exit(0);
                break;
        }
    }
    return 0;
}

void sort(int n)
{
    int i,j;
    for(i=0;i<n-1;i++)
    {
        for(j=0;j<n-1;j++)
        {
            if(tracks_copy[j]>tracks_copy[j+1])
            {
                int temp=tracks_copy[j];
                tracks_copy[j]=tracks_copy[j+1];
                tracks_copy[j+1]=temp;
            }
        }
    }
}

void copy_queue(int tracks_copy[],int n)
{
    int i=0;
    for(i=0;i<n;i++)
        tracks_copy[i]=tracks[i];
}

void fcfs_sched(int n,int head)
{
    int i,j;
    int curr=head;
    int seek=0,seekSum=0;
    for(i=0;i<n;i++)
    {
        seek=abs(curr-tracks_copy[i]);
        seekSum+=seek;
        printf("\nMove from %d to %d with SEEK: %d",curr,tracks_copy[i],seek);
        curr=tracks_copy[i];
    }
    printf("\nTotal SEEK Time:%d",seekSum);
    printf("\nAverage SEEK Time:%f",(float)seekSum/n);
}

void scan_sched(int n,int head)
{
    int i,j;
    sort(n);
    for(i=0;i<n;i++)
        printf("%d\t",tracks_copy[i]);
    int seek=0,seekSum=0;
    int curr=head;
    int currIndex=0;
    for(i=0;i<n;i++)
    {
        if(head<=tracks_copy[i])
        {
            currIndex=i;
            break;
        }
    }
    printf("\nCurr Head %d Curr Index %d",curr,currIndex);
    for(i=currIndex;i<n;i++)
    {   
        seek=abs(curr-tracks_copy[i]);
        seekSum+=seek;
        printf("\nMove from %d to %d with SEEK: %d",curr,tracks_copy[i],seek);
        curr=tracks_copy[i];
    }
    for(i=currIndex-1;i>=0;i--)
    {
        seek=abs(curr-tracks_copy[i]);
        seekSum+=seek;
        printf("\nMove from %d to %d with SEEK: %d",curr,tracks_copy[i],seek);
        curr=tracks_copy[i];
    }
    printf("\nTotal SEEK Time:%d",seekSum);
    printf("\nAverage SEEK Time:%f",(float)seekSum/n);

}

void cscan_sched(int n,int head)
{
    int i,j;
    sort(n);
    for(i=0;i<n;i++)
        printf("%d\t",tracks_copy[i]);
    int seek=0,seekSum=0;
    int curr=head;
    int currIndex=0;
    for(i=0;i<n;i++)
    {
        if(head<=tracks_copy[i])
        {
            currIndex=i;
            break;
        }
    }
    printf("\nCurr Head %d Curr Index %d",curr,currIndex);
    for(i=currIndex;i<n;i++)
    {   
        seek=abs(curr-tracks_copy[i]);
        seekSum+=seek;
        printf("\nMove from %d to %d with SEEK: %d",curr,tracks_copy[i],seek);
        curr=tracks_copy[i];
    }
    curr=0;
    for(i=0;i<currIndex;i++)
    {
        seek=abs(curr-tracks_copy[i]);
        seekSum+=seek;
        printf("\nMove from %d to %d with SEEK: %d",curr,tracks_copy[i],seek);
        curr=tracks_copy[i];
    }
    printf("\nTotal SEEK Time:%d",seekSum);
    printf("\nAverage SEEK Time:%f",(float)seekSum/n);
}