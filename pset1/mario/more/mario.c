#include<stdio.h>
int main(void)
{
    int n;
    scanf("how long do you want the pyramid : %d",&n);
    int x=n;
    int y;
    for(int i=0;i<n;i++)
    {
        for(int k=0;k<x;k++)
        {
            printf("  ");
        }
        y=n-x;
        x--;
        for(int j=0;j<y;j++)
        {
            printf("__");
        }
        printf("  ");
        for(int l=0;l<y;l++)
        {
            printf("__");
        }
        printf("\n");
    }
    return 0;
}
