#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include "btree.h"

int readfromfile(FILE *fin, BTA *dict, BTA *sdex);

void Add(BTA *dict);
void Search(BTA *dict, BTA *sdex);
void Delete(BTA *dict);

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

    char value[10000];
    int size;
    int check = btsel(sdex, soundexCode, value, 100000, &size); //check if code exists inside sdex.db
    
    if (check == 0) { //if code exists -> add word to value
        strcat(value, "\n");
        strcat(value, word);

        btupd(sdex, soundexCode, value, strlen(value) + 1);
    } else { //if not exists -> insert code & word
        strcpy(value, "you mean: \n");
        strcat(value, word);
        btins(sdex, soundexCode, value, strlen(value) + 1);
    }
    return sdex;
}

// --------------------------------------------------------------------

int readfromfile (FILE *fin, BTA *dict, BTA *sdex) {
    char key[50]; 
    int i = 0, check;

    fin = fopen("/usr/share/dict/words","r");
    if(fin == NULL){
        printf("Can not open file to read\n");
        return -1;
    }

    while(fgets(key, 50, fin)!=NULL){
        key[strlen(key) - 1] = '\0';
        printf("%s\n", key);
        check = binsky(dict, key, i);
        sdex = xulysoundex(sdex, key);
        i++;
    }

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
    scanf("%s",add);

    int value;
    int res = bfndky(dict, add, &value);
    if(res == 0){
        printf("The word has already existed in the dictionary.\n");
        printf("%s - %d\n\n", add, value );
    }
    else{
        int check = binsky(dict, add, 99999 + j);
        j++;
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
    
    int value = 0;
    int res = bfndky(dict, search, &value);
    if(res == 0) {
        printf("Result : %d\n", value );
    }
    else { //suggestion
        char code[100];
        char value[100000];
        int size;

        soundex(search, code);
        int check = btsel(sdex, code, value, 100000, &size);
        printf("%s\n", value);
    }  
}

void Delete (BTA *dict) {
	char del[50];
    printf("Input a word to delete : ");
    scanf("%s",del);

    int value;
    int res = bfndky(dict, del, &value);
    if(res == 0){
        bdelky(dict, del);
        printf("Word deleted\n");
    } else { 
        printf("Word doesnt exist in the dictionary!\n");
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
  	readfromfile(fin, dict, sdex); //read from file to btree dictionary.db and dsoundex.db

    // MENU
  	int choice;
		printf("\tMENU\n");
		printf("1. Add\n");
		printf("2. Search\n");
		printf("3. Delete\n");
		printf("4. Exit\n");

		do {
			printf("\nInput a choice: ");
			scanf("%d", &choice);

			switch(choice){
				case 1: 
					Add(dict);
					break;

				case 2: 
					Search(dict, sdex);
					break;

				case 3: 
					Delete(dict);
					break;

				case 4:
					printf("Program closing...\n");
					break;

				default:
                	printf("Invalid task Try again!\n\n");
                	break;
			}
		}	while (choice != 4);

  	btcls(dict);
    btcls(sdex);
    printf("Closed!\n");
	return 0;
}