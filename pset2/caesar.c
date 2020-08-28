#include<stdio.h>
#include<cs50.h>
#include<string.h>
void cipher(int a);
int main(int k,char *a[])
{
    if (k>2&&k<2)
    {
        printf("error , enter a correct value pf key.");
        return 1;
    }
    else
    {
        int i= atoi(a[1]);
        if (i<=26&&i>=1)
        {
            cipher(i);
        }
        else
        {
            int n=0;
            n=i/26;
            i=i-(n*26);
            cipher(i);
        }
    }
}
void cipher(a)
{
    string s=get_string("enter a sentence to cipher :");
    int n=strlen(s);
    for(int j=0;j<n;j++)
    {
        s[j]+=a;
    }
    printf("cipher text :%s",s);
}