#include<stdio.h>
#include<cs50.h>
#include<math.h>

int main(void)
{
    long int n;
    int sum=0;
    int sum1=0;
    int sum2=0;
    long int R=0,y=0;
    while(1)
    {
        n= get_long_long("enter your card number :");
        if(n>0)
        {
            break;
        }
        else
        {
            continue;
        }
    }
    for(int i=1;i<=13;i+=2)
    {
        R=n/pow(10,i);
        sum1+=(2*(R%10));
    }
    for(int J=0;J<=15;J+=2)
    {
        y=n/pow(10,J);
        sum2+=(y%10);
    }
    sum=sum1+sum2;
    if (sum%10==0)
    {
        printf("VISA\n");
    }
    else printf("INVALID\n");
}