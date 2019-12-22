#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "suggestion1.h"


//======get soundex values of input
void getsoundex(char* in, char* out){
    int wordLen = strlen(in), i, j=1, maxLen = 4;
    char maps[] = "01230120022455012623010202";
    char c;
    out[0] = toupper(in[0]);
    for(i=1; i<wordLen; i++){
        c = toupper(in[i]);
        if(c-65>=0&&c-65<=25){
            if(maps[c-65]!=maps[toupper(in[i-1])-65])
                if(maps[c-65]!='0'){
                    out[j] = maps[c-65];
                    j++;
                }
        }
        if(j>3) break;
    }
    if(j<3)
        while(j<=3){
            out[j]='0';
            j++;
        }
}

//=======Initial dict
Dict *buildDict(){
    Dict *dict = (Dict *)malloc(sizeof(Dict));
    dict->size = INITIAL_SIZE;
    dict->total = 0;
    dict->entries = (wordEntry *)malloc(sizeof(wordEntry)*INITIAL_SIZE);
    // printf("good\n");
    return dict;
}

//=======add element
void addWord(Dict* dict, char* val){
    // printf(" food\n");
    char soundex[5] = " ";
    getsoundex(val, soundex);
    wordEntry *entries = dict->entries;
    //printf("%s %s\n", val, soundex);
    // printf(" food\n");
    strcpy(entries[dict->total].val, val);
    // printf(" food\n");
    strcpy(entries[dict->total].soundex, soundex);
    dict->total++;
}

void addFromFile(Dict* dict, char* val, char* soundex){
    wordEntry *entries = dict->entries;
    strcpy(entries[dict->total].val, val);
    strcpy(entries[dict->total].soundex, soundex);
    dict->total++;
}

//======close Dict
void dropDict(Dict* dict){
    printf("good\n");
    free(dict->entries);
    free(dict);
}

void printDict(Dict *dict){
    for(int i = 0; i<dict->total;i++){
        printf("string: %s \t\t Soundex value: %s \n", dict->entries[i].val,dict->entries[i].soundex);
    }
}

void getSug(char* key, Dict *dict){
    int i, num=0;
    char keySoEx[5] = "";
    getsoundex(key, keySoEx);
    char res[60];
    printf("You may want to find: \n");
    for(i=0; i<dict->total; i++){
        if(strcmp(keySoEx, dict->entries[i].soundex)==0){
            strcpy(res,dict->entries[i].val);
            printf("%-16s \n", res);
            num++;
        }
        if(num>10) break;
    }
    if(num==0)
        printf("No result available\n");
}