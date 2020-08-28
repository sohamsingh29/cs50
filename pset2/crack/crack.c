#include<stdio.h>
#include<cs50.h>
#include<string.h>
#define _XOPEN_SOURCE
#include <unistd.h>
void decipher(string hp,string s);
string crypt(string key,string salt);
int main(int k,string a[])
{
    string t= a[1];
    char salt[2];
    if (k>2&&k<2)
    {
        printf("error , enter a correct value of key.");
        return 1;
    }
    else
    {
        salt[0]=t[0];
        salt[1]=t[1];
        decipher(t,salt);
    }
}
void decipher(string hp,string s)
{
    char pass[5];
    string hashed;
    string alphabet="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    //for five characters
    for(int i=0;i<52;i++)
    {
        pass[0]=alphabet[i];
        hashed=crypt(pass,s);
        if(strcmp(hashed,hp)==0)
        {
            goto print;
        }
        for(int j=0;j<52;j++)
        {
            pass[1]=alphabet[i];
            hashed=crypt(pass,s);
            if(strcmp(hashed,hp)==0)
            {
                goto print;
            }
            for(int k=0;k<52;k++)
            {
                pass[2]=alphabet[i];
                hashed=crypt(pass,s);
                if(strcmp(hashed,hp)==0)
                {
                    goto print;
                }
                for(int l=0;l<52;l++)
                {
                    pass[3]=alphabet[i];
                    hashed=crypt(pass,s);
                    if(strcmp(hashed,hp)==0)
                    {
                        goto print;
                    }
                    for(int m=0;m<52;m++)
                    {
                        pass[4]=alphabet[i];
                        hashed=crypt(pass,s);
                        if(strcmp(hashed,hp)==0)
                        {
                            goto print;
                        }
                    }
                }
            }
        }
    }
    print: printf("deciphered text : %s",pass);
}