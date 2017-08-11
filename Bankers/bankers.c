#include<stdio.h>

int main()
{
    int max[20][20],alloc[20][20],need[20][20];
    int m,n,i,j,k;
    printf("\nEnter Number of Processes:");
    cin>>n;
    printf("\nEnter Number of Resources:");
    cin>>m;
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
}