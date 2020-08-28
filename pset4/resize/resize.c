// resize an  image
#include <stdio.h>
#include <stdlib.h>
#include<math.h>
#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize g infile outfile\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];
    float g = atof(argv[1]);
    if (g>100.0||g<0)
    {
        fprintf(stderr,"cannot be resized \n");
        return 7;
    }
    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf,bfOut;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    bfOut = bf;


    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi,biOut;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);
    biOut = bi;


    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    biOut.biWidth = floor(bi.biWidth*g);

    biOut.biHeight = floor((bi.biHeight>0?-bi.biHeight:bi.biHeight)*g);

    int padding = ( 4 -  ( bi.biWidth*sizeof(RGBTRIPLE) ) % 4  ) % 4;

    int outPadding = (4 - (biOut.biWidth * sizeof(RGBTRIPLE)) % 4 )% 4;

    biOut.biSizeImage = ((sizeof(RGBTRIPLE) * biOut.biWidth) + outPadding) * abs(biOut.biHeight);

    bfOut.bfSize = biOut.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);


    // write outfile's BITMAPFILEHEADER
    fwrite(&bfOut, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&biOut, sizeof(BITMAPINFOHEADER), 1, outptr);

    int cols = abs(bi.biHeight);

    RGBTRIPLE inrgb[cols][bi.biWidth];

    int i,j,k,ini,inj,l;


    // to read from inptr
    if(bi.biHeight < 0)
    {

        for (i = 0; i < abs(bi.biHeight); i++)
        {

            // iterate over pixels in scanline
            for (j = 0; j < bi.biWidth; j++)
            {
                // read RGB triple from infile
                fread(&(inrgb[i][j]), sizeof(RGBTRIPLE), 1, inptr);
            }

            // skip over padding, if any
            fseek(inptr, padding, SEEK_CUR);
        }
    }
    else
    {
        for (i = abs(bi.biHeight) - 1; i >= 0; i--)
        {

            // iterate over pixels in scanline
            for (j = 0; j < bi.biWidth; j++)
            {
                // read RGB triple from infile
                fread(&(inrgb[i][j]), sizeof(RGBTRIPLE), 1, inptr);
            }

            // skip over padding, if any
            fseek(inptr, padding, SEEK_CUR);
        }
    }
    // to put values in outpixels
    cols = abs(biOut.biHeight);

    RGBTRIPLE outPixel[cols][biOut.biWidth];

    for ( i = 0; i<abs(bi.biHeight); i++)
    {
        ini = floor(i*g);
        for( j = 0; j<bi.biWidth; j++)
        {
            inj = ceil(j*g);
            outPixel[ini][inj] = inrgb[i][j];
            for( k = floor(g) - 1; k > 0; k--)
            {
                outPixel[ini][inj + k] = inrgb[i][j];
            }
        }
        for( l = floor(g) - 1; l > 0; l--)
        {
            for(j = 0; j < bi.biWidth; j++)
            {
                inj = ceil(g*j);
                outPixel[ini + l][inj] = inrgb[i][j];
                for(k = floor(g) - 1; k > 0; k--)
                {
                    outPixel[ini + l][inj + k] = inrgb[i][j];
                }
            }
        }
    }

    for (i = 0; i < abs(biOut.biHeight); i++)
    {
        // iterate over pixels in scanline
        for (j = 0; j < biOut.biWidth; j++)
        {
            fwrite(&(outPixel[i][j]), sizeof(RGBTRIPLE), 1, outptr);
        }

        // padding
        for (k = 0; k < outPadding; k++)
        {
            fputc(0x00, outptr);
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
