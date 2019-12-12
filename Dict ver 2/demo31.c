#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include "btree.h"

int readfromfile(FILE *fin, BTA *dict, BTA *sdex);

void Add(BTA *dict);
void Search(BTA *dict, BTA *sdex);
void Delete(BTA *dict, BTA *sdex);

void soundex(char *word, char *result);
void xulisoundex(BTA *sdex, char *word);


// --------------------------------------------------------------------

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
}

void *xulysoundex(BTA *sdex, char *word) {
    
    char soundexCode[100];
    soundex(word, soundexCode); //word -> soundex code

    char value[100000];
    int size;
    int check = btsel(sdex, soundexCode, value, 1000000, &size); //check if code exists inside sdex.db
    
    if (check == 0) { //if code exists -> add word to value
        strcat(value, "\t");
        strcat(value, word);

        btupd(sdex, soundexCode, value, strlen(value) + 1);
    } else { //if not exists -> insert code & word
        strcpy(value, word);
        //strcat(value, word);
        btins(sdex, soundexCode, value, strlen(value) + 1);
    }
    return sdex;
}

// --------------------------------------------------------------------

int readfromfile (FILE *fin, BTA *dict, BTA *sdex) {
    char key[50]; 
    int check;

    fin = fopen("dict.txt","r");
    if(fin == NULL){
        printf("Can not open file to read\n");
        return -1;
    }

    int total = 0;
    while(fgets(key, 50, fin)!=NULL){
        key[strlen(key) - 1] = '\0';
        total++;
        //printf("%s - ", key);
        char value[10000] = "Nghia cua tu ";
        strcat(value, key);
        //printf("%s\n", value);
        check = btins(dict, key, value, strlen(value) + 1);
        sdex = xulysoundex(sdex, key);
    }
    printf("Total words: %d\n", total);

    if (check == 0) printf("dictionary loaded!\n");
    else {
        printf("unable to load dictionary!\n");
        printf("Try to delete the dictionary.db and try again.\n");
        exit(0);
    }
}

// --------------------------------------------------------------------

int j = 0;
void Add (BTA *dict) {
	char add[50];
    printf("Input a word to add : ");
    scanf("%s", add);

    char str[1000] = "Nghia cua tu ";
    int size; char value[100000];
    int res = btsel(dict, add, value, 100000, &size);
    if(res == 0){
        printf("The word has already existed in the dictionary.\n");
        printf("%s - %s\n\n", add, value);
    }
    else{
        strcat(str, add);
        int check = btins(dict, add, str, strlen(str) + 1);
        if(check == 0){
            printf("word added successfully!\n");
        } else {
        	printf("adding fail\n");
        }
    }
}

void Search (BTA *dict, BTA *sdex) {
	char search[50];
    printf("Input a word to search : ");
    scanf("%s", search);    
    
    char value[100000];
    int size;
    int res = btsel(dict, search, value, 100000, &size);
    if(res == 0) {
        printf("Result : %s\n", value);
    }
    else { //suggestion
        char code[100];
        char value2[100000];

        soundex(search, code);
        int check = btsel(sdex, code, value2, 100000, &size);
        printf("%s\n", value2);
    }  
}

void Delete (BTA *dict, BTA *sdex) {
	char del[50];
    printf("Input a word to delete : ");
    scanf("%s",del);

    char value[100000];
    int size;
    int res = btsel(dict, del, value, 100000, &size);
    if(res == 0){
        bdelky(dict, del);
        printf("Word deleted\n");
    } 
    else { //suggestion
        printf("Word doesnt exist in the dictionary!\n");
        char code[100];
        char value2[100000];

        soundex(del, code);
        int check = btsel(sdex, code, value2, 100000, &size);
        printf("%s\n", value2);
    }
}

// -----------------------------------------------------------------------

int main() {
	char d[] = "dictionary.db";
    char s[] = "dictionarySoundex.db";
	btinit();

    //create dictionary db
	BTA *dict = btopn(d, 0, 0);
    if (dict == NULL)
        dict = btcrt(d, 0, 0);

    //create soundex db
    BTA *sdex = btopn(s, 0, 0);
    if (sdex == NULL)
        sdex = btcrt(s, 0, 0);

    FILE *fin;
    printf("\tDICTIONARY\n\nCREATING DATABASE ...\n\n");
  	readfromfile(fin, dict, sdex); //read from file to btree dictionary.db and dsoundex.db

    // MENU
  // 	int choice;
		// printf("\tMENU\n");
		// printf("1. Add\n");
		// printf("2. Search\n");
		// printf("3. Delete\n");
		// printf("4. Exit\n");

		// do {
		// 	printf("\nInput a choice: ");
		// 	scanf("%d", &choice);

		// 	switch(choice){
		// 		case 1: 
		// 			Add(dict);
		// 			break;

		// 		case 2: 
		// 			Search(dict, sdex);
		// 			break;

		// 		case 3: 
		// 			Delete(dict, sdex);
		// 			break;

		// 		case 4:
		// 			printf("Program closing...\n");
		// 			break;

		// 		default:
  //               	printf("Invalid task Try again!\n\n");
  //               	break;
		// 	}
		// }	while (choice != 4);

  	btcls(dict);
    btcls(sdex);
    printf("DONE!\n");
	return 0;
}