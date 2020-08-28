// Helper functions for music
#include<string.h>
#include <cs50.h>
#include<math.h>
#include<stdlib.h>
#include<ctype.h>
#include "helpers.h"

// Converts a fraction formatted as X/Y to eighths
int duration(string fraction)
{
    int n= fraction[0] - '0';
    int d= fraction[2] - '0';

    return n * (8/d);
}

// Calculates frequency (in Hz) of a note
int frequency(string note)
{
    int n = 0,octave=0;

    switch(note[0])
    {
        case 'C': n=-9;

        case 'D': n=-7;

        case 'E': n=-5;

        case 'F': n=-4;

        case 'G': n=-2;

        case 'A': n=0;

        case 'B': n=2;
    }

        if(strlen(note)==3)
        {
            octave = note[2] - '0';
        }
        else
        {
            octave = note[1] - '0';
        }

        octave-=4;

        n+=12*octave;

        if(strlen(note)==3)
        {
            if(strcmp(&note[1],"#")==0)
            {
                n++;
            }
            else
            {
                n--;
            }
        }

        return 440*pow(2,n/12);
}

// Determines whether a string represents a rest
bool is_rest(string s)
{
    return strlen(s)==0;
}