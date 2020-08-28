#include<stdio.h>
#include<string.h>
int main(void)
{
    FILE* fp = fopen("card.raw","r");
    if (fp==NULL)
    {
        fclose(fp);
        printf("unable to open file\n");
        return 1;
    }
    unsigned char buffer[512];
    int imgcount=0;
    int isjpeg=0;
    FILE* fk=NULL;
    char filename[8];
    while(fread(buffer,512,1,fp)==1)
    {
        if((buffer[0] == 0xff )&& (buffer[1] == 0xd8 )&& (buffer[2] == 0xff) && ((buffer[3] & 0xe0) == 0xe0))
        {
            if(isjpeg==1)
            {
                fclose(fk);
            }
            else
            {
                isjpeg=1;
            }
            sprintf(filename,"%03d.jpg",imgcount);
            fk=fopen(filename,"w");
            imgcount++;
        }
        if(isjpeg==1)
        {
            fwrite(&buffer,512,1,fk);
        }
    }
    fclose(fk);
    fclose(fp);
    return 0;
}