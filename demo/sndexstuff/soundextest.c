#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void soundex(char *word, char *s) {
    int si = 1;
    char c;
    //                 ABCDEFGHIJKLMNOPQRSTUVWXYZ
    char mappings[] = "01230120022455012623010202";

    s[0] = toupper(word[0]);

    for(int i = 1, l = strlen(word); i < l; i++){
        c = toupper(word[i]) - 65;

        if (c >= 0 && c <= 25) {
            if (mappings[c] != '0') {
                if (mappings[c] != s[si-1]) {
                    s[si] = mappings[c];
                    si++;
                }
                if(si > 3)
                    break;
            }
        }
    }

    if(si <= 3)
        while(si <= 3) {
            s[si] = '0';
            si++;
        }
}

int main() {
    char *name = "buki";
    char *name2 = "backache";
    char res[10], res2[10];
    soundex(name, res);
    soundex(name2, res2);
    printf("%s - %s\n", res, res2);


}