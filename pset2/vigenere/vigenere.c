#include<stdio.h>
#include<cs50.h>
#include<string.h>
#include<ctype.h>
void cipher(int h[],int o);
int main(int k,string a[])
{
    string t=a[1];
    int n=strlen(t);
    int f[n];
    if (k>2&&k<2&&(isalpha(a[1])==0))
    {
        printf("error , enter a correct value key.");
        return 1;
    }
    else
    {
        for(int j=0;j<n;j++)
        {
            f[j]=t[j];
            if(f[j]>=97)
            {
                f[j]-=97;
            }
            else if(f[j]>=65)
            {
                f[j]-=65;
            }
        }
        cipher(f,n);
    }
    return 0;
}
void cipher(int h[],int o)
{
    string s=get_string("plaintext :");
    int m=strlen(s);
    int b=0;
    for (int j=0;j<m;j++)
    {
        if(isalpha(s[j]))
        {
            if(b>=o)
            {
                b=0;
            }
            s[j]+=h[b];
            if(!isalpha(s[j]))
            {
                if(s[j]>=123)
            {
                s[j]-=97;
                s[j]%=26;
                s[j]+=97;
            }
            else if(s[j]>=92)
            {
                s[j]-=65;
                s[j]%=26;
                s[j]+=65;
            }
            }
            b++;
        }
    }
    printf("ciphertext :%s",s);
}