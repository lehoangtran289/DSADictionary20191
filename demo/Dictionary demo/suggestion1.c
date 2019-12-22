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

//======sort====

int swap(Dict *dict, int left, int right){
    char val[60], soundex[5];
    //=======get temp
    strcpy(val, dict->entries[left].val);
    strcpy(soundex, dict->entries[left].soundex);
    //=======swap left right
    strcpy(dict->entries[left].val, dict->entries[right].val);
    strcpy(dict->entries[left].soundex, dict->entries[right].soundex);
    strcpy(dict->entries[right].val, val);
    strcpy(dict->entries[right].soundex, soundex);
    return 0;
}

int sortDict(Dict *dict, int left, int right){
    int i = left-1, j = right, p = left-1, q = right, k;
    char key[5]; 
    strcpy(key, dict->entries[right].soundex);
    if(left>=right) return 0;
    while(1){
        while(strcmp(dict->entries[++i].soundex,key)<0);
        while(strcmp(dict->entries[--j].soundex, key)>0) if(j==left) break;
        if(i>=j) break;
        swap(dict, i, j);
        if(strcmp(dict->entries[i].soundex, key)==0){
            p++;
            swap(dict, p, i);
            // p++;
        }
        if(strcmp(dict->entries[j].soundex, key)==0){
            q--;
            swap(dict, q, j);
            // q--;
        }
    }
    swap(dict, i, right);
    j = i-1; i++;
    for(k=left; k<p; k++, j--) swap(dict, k, j);
    for(k=right-1; k>q; k--, i++) swap(dict, i, k);
    sortDict(dict, left, j);
    sortDict(dict, i, right);
    return 0;
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

//======read from file ======

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

//=======
void printDict(Dict *dict){
    for(int i = 0; i<dict->total;i++){
        printf("string: %s \t\t Soundex value: %s \n", dict->entries[i].val,dict->entries[i].soundex);
    }
}

//=======get suggestion========
void getSug(char* key, Dict *dict){
    int i, num=0;
    char keySoEx[5] = "";
    getsoundex(key, keySoEx);
    char res[60];
    printf("You may want to find: \n");
    for(i=0; i<dict->total; i++){
        if(strcmp(keySoEx, dict->entries[i].soundex)==0){
            strcpy(res,dict->entries[i].val);
            printf("%-16s %s\n", res, dict->entries[i].soundex);
            num++;
        }
        if(num>10) break;
    }
    if(num==0)
        printf("No result available\n");
}

//========get sug using binary====

void binarySug(char* key, Dict *dict){
    int i, num=0;
    char keySoEx[5] = "";
    getsoundex(key, keySoEx);
    char res[60];
    printf("You may want to find: \n");
    int mid, lo = 0; int high = dict->total -1;
    while(lo<high){
        mid = (lo+high)/2;
        if(strcmp(keySoEx, dict->entries[mid].soundex)==0){
            printf("%s\n", dict->entries[mid].val);
            for(i=1; i<=5; i++){
                if(strcmp(keySoEx, dict->entries[mid-i].soundex)==0)
                    printf("%-30s\n", dict->entries[mid-i].val);
                if(strcmp(keySoEx, dict->entries[mid+i].soundex)==0)
                    printf("%-30s\n", dict->entries[mid+i].val);
            }
            break;
        } else if(strcmp(keySoEx, dict->entries[mid].soundex)<0){
            high = mid-1;
        } else lo = mid+1;
        if(lo>=high){
            printf("No result available! \n");
            break;
        }
    }
}