#include<stdio.h>

void findNeed(int need[][20],int max[][20],int alloc[][20],int n,int m);
void findSafeState(int avail[],int max[][20],int alloc[][20],int n,int m);

int main()
{
    int max[20][20],alloc[20][20],avail[20];
    int m,n,i,j;
    printf("\nEnter Number of Processes:");
    scanf("%d",&n);
    printf("\nEnter Number of Resources:");
    scanf("%d",&m);
    printf("\nEnter The Demand Matrix(%d X %d): ",n,m);
    for(i=0;i<n;i++)
    {
        for(j=0;j<m;j++)
        {
            scanf("%d",&max[i][j]);
        }
    }
    printf("\nEnter The Allocation Matrix(%d X %d): ",n,m);
    for(i=0;i<n;i++)
    {
        for(j=0;j<m;j++)
        {
            scanf("%d",&alloc[i][j]);
        }
    }
    printf("\nEnter The Available Resources(%d resources):",m);
    for(i=0;i<m;i++)
        scanf("%d",&avail[i]);

    findSafeState(avail,max,alloc,n,m);

}

void findNeed(int need[][20],int max[][20],int alloc[][20],int n,int m)
{
    int i,j;
    for(i=0;i<n;i++)
    {
        for(j=0;j<m;j++)
        {
            need[i][j]=max[i][j]-alloc[i][j];
        }
    }
}

void findSafeState(int avail[],int max[][20],int alloc[][20],int n,int m)
{
    int need[20][20];
    int work[20],finish[20],safeSequence[20];
    int i,j,k;

    findNeed(need,max,alloc,n,m);
    printf("\nNeed Matrix Is: \n");
    for(i=0;i<n;i++)
    {
        for(j=0;j<m;j++)
            printf("%d ",need[i][j]);
        printf("\n");
    }
    for(i=0;i<n;i++)
        finish[i]=0;

    for(i=0;i<m;i++)
        work[i]=avail[i];
    
    int count=0;

    while(count<n)
    {
        int flag=1;
        for(i=0;i<n;i++)
        {
            if(finish[i]==0)
            {
                for(j=0;j<m;j++)
                {
                    if(need[i][j]>work[j])
                        break;
                }
                if(j==m)
                {
                    for(k=0;k<m;k++)
                    {
                        work[k]+=alloc[i][k];
                    }
                    safeSequence[count]=i;
                    count+=1;
                    finish[i]=1;
                    flag=1;
                }
            }
        }
        if(!flag)
        {
            printf("\nSystem Not in Safe State");
            return;
        }
    }
    printf("\nSafe Sequence Is: ");
    for(i=0;i<n;i++)
        printf("%d\t",safeSequence[i]);    
}
