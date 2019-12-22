#define INITIAL_SIZE 100000 
#define INCREMENTAL_SIZE 20000

typedef struct {char val[60]; char soundex[5];} wordEntry;
typedef struct {wordEntry *entries; int total; int size;} Dict;

// void getWord(char* in, wordEntry *word);
Dict *buildDict();
void dropDict(Dict* dict);
void addWord(Dict* dict, char* val);
void addFromFile(Dict* dict, char* val, char* soundex);
void getsoundex(char* in, char* out);
void printDict(Dict *dict);
void getSug(char* key, Dict *dict);

