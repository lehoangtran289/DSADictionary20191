// code by minari 
// October 2019
// demo just a demo
// check check

#include <stdio.h>
#include <stdlib.h>
#include "btree.h"
#include <string.h>
#include <ctype.h>
#include "suggestion1.h"


void printMenu(){
    printf("=====================MENU=====================\n");
    printf("1. Search\n");
    printf("2. Add\n");
    printf("3. Delete\n");
    printf("4. Exit\n");
    printf("==============================================\n");
}
void suggestion(char* search){
    Dict *soundexDict;
    soundexDict = buildDict();
    char des[] = "soundexDict.txt";
    FILE *fin;
        
        if((fin=fopen(des,"r"))==NULL){
            printf("fail to read\n");
            return -1;
        }
        char val[60], soEx[5]; int n=0;
        while(fgets(val,60,fin)!=NULL){
            val[strlen(val)-1]='\0';
            fgets(soEx,6,fin);
            soEx[strlen(soEx)-1]='\0';
            addFromFile(soundexDict, val, soEx);
            n++;
        }
        fclose(fin);
    binarySug(search, soundexDict);
    dropDict(soundexDict);
}
void Search(BTA *dict){
    char search[50];
    printf("=============SEARCH=================\n");
    printf("input the word to search : ");
    scanf("%[^\n]s",search);    
    scanf("%*c");
    int value = 0;

    int res = bfndky(dict,search,&value);
    if(res == 0){
        printf("Result : %d\n", value );
    }
    else{
        printf("Not found\n");
        suggestion(search);
    }

    printf("=====================================\n");

}

void Add(BTA *dict){
    char add[50];
    printf("===============ADD===================\n");
    printf("Input the word to add : ");
    scanf("%[^\n]s",add);
    scanf("%*c");
    int value;
    int res = bfndky(dict,add,&value);
    if(res == 0){
        printf("The word has already existed in the dictionary.\n");
        printf("%s - %d\n",add,value );
    }
    else{
        int test = binsky(dict,add,99999);
        if(test == 0){
            printf("word added successfully!\n");
        }
    }
    printf("=====================================\n");
}
void Delete(BTA *dict){
    char del[50];
    printf("===============DELETE================\n");
    printf("Input the word to delete : ");
    scanf("%[^\n]s",del);
    scanf("%*c");
    int value;
    int res = bfndky(dict,del,&value);
    if(res == 0){
        printf("Deleting...\n");
        bdelky(dict,del);
        printf("Deleted\n");
    }
    else{
        printf("Word doesnt exist in the dictionary!\n");
    }
}
int main(){
    //gcc -o demo.out demo.c -Lbt-5.0.0/lib -lbt -Ibt-5.0.0/inc
    //=====================================================
    char d[] = "dictionary.txt";

	btinit();
	BTA *dict;
    
    char x[50];
    printf("test : \n");
    scanf("%[^\n]s",x);
    scanf("%*c");
    printf("test : %s\n",x);

    printf("Loading file dictionary...\n");
	dict = btopn(d,0,0);
    int test;
	if(dict == NULL){
        dict = btcrt(d,0,0);
        FILE *p;
        if((p=fopen("/usr/share/dict/words","r"))==NULL){
            printf("fail to read\n");
            return -1;
        }
        char temp[50]; 
        int i = 0;
        while(fgets(temp,50,p)!=NULL){
            temp[strlen(temp)-2]='\0';
            // printf("%s\n",temp );
            test = binsky(dict,temp,i);
            i++;
        }
        fclose(p);
    }
    if(test == 0){
        printf("dictionary loaded!\n");
    }
    else{
        printf("unable to load dictionary!\n");
        printf("Try to delete the dictionary.db and try again.\n");
        return 0;
    }

    //=====================================================
    unsigned choice;
    char search[50];
    do{
        printMenu();
        printf("Input your choice : ");
        scanf("%d",&choice);
        scanf("%*c");
        switch(choice){
            case 1: 
                    // printf("=============SEARCH=================\n");
                    // printf("input the word to search : ");
                    // scanf("%[^\n]s",search);    
                    // scanf("%*c");
                    Search(dict);
                    // suggestion(search);
                    break;
            case 2: Add(dict);
                    break;
            case 3: Delete(dict);
                    break;
            case 4: printf("Quitting....\n");
                    break;
            default : printf("invalid choice, re input : ");
                    scanf("%d",&choice);
                    if(choice == 4) printf("Quitting....\n");
        }
    }while(choice!= 4);


    //=====================================================
    printf("Closing dictionary...\n");
    btcls(dict);
    printf("Closed!\n");
    return 0;
}
