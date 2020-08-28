#include<cs50.h>
#include<stdio.h>

int main(void)
{
    float n;
    int c;
    int y;
    while(1)
    {
        n= get_float("change owed :");
        if(n>0)
        {
            break;
        }
        else
        {
            continue;
        }
    }
    c=n/0.25;
    n=n-(c*0.25);
    y=n/0.10;
    c=c+y;
    n=n-(y*0.10);
    y=n/0.05;
    c=c+y;
    n=n-(y*0.05);
    y=n/0.01;
    c=c+y;
    printf("the amount of coins : %d",c);
}