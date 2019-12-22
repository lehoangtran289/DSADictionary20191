// code by minari 
// October 2019
// demo just a demo

#include <stdio.h>
#include <stdlib.h>
#include "btree.h"
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

BTA *dict,*sdex;
char *array[100];

void printMenu(){
    printf("=====================MENU=====================\n");
    printf("1. Search - Autocomplete\n");
    printf("2. Search - Soundex\n");
    printf("3. Add\n");
    printf("4. Delete\n");
    printf("5. Exit\n");
    printf("==============================================\n");
}
//============================================================
void soundex(char *word, char *result) {
	int si = 1;
    char c;
    //                 ABCDEFGHIJKLMNOPQRSTUVWXYZ
    char mappings[] = "01230120022455012623010202";

    result[0] = toupper(word[0]);

    for(int i = 1, l = strlen(word); i < l; i++){
        c = toupper(word[i]) - 65;
        if (c >= 0 && c <= 25) {
            if (mappings[c] != '0') {
                if (mappings[c] != result[si-1]) {
                    result[si] = mappings[c];
                    si++;
                }
                if(si > 3)
                    break;
            }
        }
    }
    if(si <= 3)
        while(si <= 3) {
            result[si] = '0';
            si++;
        }
    result[si] = '\0';
}
BTA *xulysoundex(BTA *sdex, char *word) {
    FILE *data;
    FILE *test;
    test = fopen("test.txt","a");
    char folder[] = "soundex/";
    char soundexCode[100];
    soundex(word, soundexCode); //word -> soundex code

    char value[10000];
    int size;
    int check = btsel(sdex, soundexCode, value, 100000, &size); //check if code exists inside sdex.db
    char link[100];
    strcpy(link,folder);
    strcat(link,soundexCode);
    fprintf(test,"%s\n",link);

    
    if (check == 0) { //if code exists -> add word to value
        if((data = fopen(link,"a"))==NULL){
            printf("Error open file data %s",link);
        }
        fprintf(data,"%s\n",word);
    } else { //if not exists -> insert code & word
        if((data = fopen(link,"w"))==NULL){
            printf("Error open file data %s\n",link);
        }
        check = btins(sdex,soundexCode, link, strlen(link)+1);
        if (check != 0){
            printf("nhap du lieu loi !\n");
        }
        fprintf(data,"%s\n",word);
    }
    fclose(data);
    fclose(test);
    return sdex;
}
void readsoundex(char link[], char *array[],int *size){
    FILE *p;
    if((p = fopen(link,"r"))==NULL){
        printf("Error reading file data %s\n",link);
    }
    char temp[50];
    *size =0;
    while(fgets(temp,50,p)!=NULL){
        temp[strlen(temp)-1]='\0';
        array[*size] = strdup(temp);
        *size = *size + 1;
    }
    fclose(p);
}
void SearchSoundex(BTA *dict, BTA *sdex) {
	char search[50];
    printf("Input a word to search : ");
    scanf("%s", search);    
    
    char value[100];
    int size;
    // int res = bfndky(dict, search, &value);
    int res = btsel(dict,search,value, 100000,&size);
    if(res == 0) {
        printf("Result : %s\n", value);
    }
    else { //suggestion
        char code[100];
        char link[100];
        char *array[100]; 

        soundex(search, code);
        // printf("check code : %s\n",code);
        int check = btsel(sdex, code, link, 100000, &size);

        printf("check : %d\n",check);
        printf("test link : %s\n",link);
        if (check == 0){
            printf("<You mean\\> \n");
            readsoundex(link,array,&size);
            for(int i = 0; i< size;i++ ){
                printf("%s\n",array[i]);
            }
        }
        else{
            printf("No suggestion too.\n");
        }
    }  
}
char * word_generator(const char*search, int state){
    char *pointer;
    char code[100];
    char link[100];
    int size;
    int list_index, len;
    soundex((char*)search,code);

    int check  = btsel(sdex, code, link, 100000, &size);
    if(check != 0){
        printf("da co loi : \n");
        printf("link : %s\n",link);
        printf("code : %s\n",code);
        printf("search : %s\n",search);
    }
    if (!state){
        list_index = 0;
        len = strlen(search);
    }
    readsoundex(link,array,&size);
    while((pointer = array[list_index++])){
        if(strncmp(pointer,search,len)==0){
            return strdup(pointer);
        }
    }
    return NULL;

}
char ** word_complete(const char *search, int start, int end){
    rl_attempted_completion_over = 1;
    return rl_completion_matches(search,word_generator);
}
void Search(BTA *dict, BTA *sdex){
    printf("========SEARCH - AUTOCOMPLETE========\n");
    
    rl_attempted_completion_function = word_complete;
    char *input = readline("<Search\\> ");
    if (input){
        printf("Result : %s\n",input);
        free(input);
    }
    else {
        printf("No result ... \n");
        free(input);
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
    // int res = bfndky(dict,add,&value);
    int res = btsel(dict, add, add, sizeof(add), &value);
    if(res == 0){
        printf("The word has already existed in the dictionary.\n");
        printf("%s - %d\n",add,value);
    }
    else{
        int test = btins(dict,add,add,sizeof(add));
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
    // int res = bfndky(dict,del,&value);
    int res = btsel(dict, del, del, sizeof(del), &value);
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
    //gcc -o demo.out demo.c -Lbt-5.0.0/lib -lbt -Ibt-5.0.0/inc -lreadline
    //=====================================================
    char d[] = "dictionary.txt";
    char s[] = "dictionarySoundex.txt";

    // BTA *dict,*sdex;
	btinit();

    printf("Loading file dictionary...\n");
	dict = btopn(d,0,0);
    sdex = btopn(s,0,0);
    int test = 0;
	if(dict == NULL || sdex == NULL){
        printf("Unable to find the dictionary file, trying to reload...\n");
        dict = btcrt(d,0,0);
        sdex = btcrt(s,0,0);
        FILE *p;
        if((p=fopen("dict.txt","r"))==NULL){
            printf("fail to read\n");
            return -1;
        }
        char temp[50]; 
        int i = 0;
        while(fgets(temp,50,p)!=NULL){
            temp[strlen(temp)-1]='\0';
            test = btins(dict, temp, temp, sizeof(temp));
            sdex = xulysoundex(sdex,temp);
            i++;
        }
        fclose(p);  
    }
    if(test == 0){
        printf("dictionary loaded!\n");
    }
    else{
        printf("Error to load dictionary!\n");
        printf("Try to delete the dictionary.txt and try again.\n");
        return 0;
    }


    // printf("========SEARCH - AUTOCOMPLETE========\n");
    
    // rl_attempted_completion_function = word_complete;
    // char *input = readline("<Search\\> ");
    // if (input){
    //     printf("Result : %s\n",input);
    //     free(input);
    // }

    // printf("=====================================\n");
    //=====================================================
    unsigned choice;
    do{ 
        printMenu();
        printf("Input your choice : ");
        scanf("%d",&choice);
        scanf("%*c");
        switch(choice){
            case 1: Search(dict,sdex);
                    break;
            case 2: SearchSoundex(dict,sdex);
                    break;
            case 3: Add(dict);
                    break;
            case 4: Delete(dict);;
                    break;
            case 5: printf("Quitting....\n");
                    break; 
            default : 
                    break;
        }
    }while(choice!= 5);


    //=====================================================
    printf("Closing dictionary...\n");
    btcls(dict);
    btcls(sdex);
    printf("Closed!\n");
    return 0;
}