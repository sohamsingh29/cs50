// Implements a dictionary's functionality

#include <stdbool.h>
#include<stdio.h>
#include "dictionary.h"
#include<stdlib.h>

#define ASCII 97
#define alphaSize 26

typedef struct node  //defining datastructure trie for dictionary
{
    struct node *alphabet[27]; //for 26 alphabets and apostrophe

    bool isWord;

}node;

int dictionarySize=0; //keep tracks of the dictionary size

node *first= NULL; //root node

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    int i = 0;
    int c = 0;
    node* nextNode = first;
    while(c!='\0')
    {
        c = word[i];
        if(c <= 90 && c >= 65) {
            c |= 32;
        }


        if((c >= 97 && c <= 122) || (c == '\''))
        {

            if(c == '\'')
                c = alphaSize + ASCII;

            if(nextNode->alphabet[c - ASCII] == NULL)
            {
                return false;
            }
            else
            {
                nextNode = nextNode->alphabet[c - ASCII];
            }
        }

        i++;
    }
    return true;
}

node* createNode()
{
    return (node*) malloc(sizeof(node));
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE* dictionaryFile = fopen(dictionary,"r");

    if(dictionaryFile==NULL)
    {
        printf("could not open file.\n");
        return false;
    }

    first = createNode();

    node* nextNode = first;

    int c =0;

    while(c!= EOF)
    {
        c= fgetc(dictionaryFile);
        if(c!=EOF && c!='\n')
        {
        if (c=='\'')
        {
            c = alphaSize + ASCII;
        }
        if(nextNode->alphabet[c - ASCII] == NULL)
        {
            nextNode->alphabet[c - ASCII] = createNode();
        }
            nextNode=nextNode->alphabet[c - ASCII];
        }
        if(c == '\n')
        {
            nextNode->isWord = true;
            dictionarySize++;
            nextNode = first;
        }
    }
    fclose(dictionaryFile);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return dictionarySize;
}
void unloadNode(node* nextNode)
{
    for ( int i =0; i<=alphaSize;i++)
    {
        if(nextNode->alphabet[i]!=NULL)
        {
            unloadNode(nextNode->alphabet[i]);
        }
    }
    free(nextNode);
}
// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    unloadNode(first);
    return true;
}
