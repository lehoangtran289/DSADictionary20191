#include <stdio.h>
#include <string.h>
#include <ctype.h>

int validate(char test[]){
    if (strlen(test) == 1)
        return 0; 
    for (int i =0 ;i<strlen(test);i++){
        char c = test[i];
        if (!((c>=97&&c<=122)||(c>=65&&c<=90))){
            return 0;
        }
    }
    return 1;
}
int main(){
    printf("vao day");
    FILE *p,*q;
    printf("vao day");
    if((p=fopen("/usr/share/dict/words","r"))==NULL){
        printf("fail to read\n");
        return -1;
    }
    if((q=fopen("dict.txt","w"))==NULL){
        printf("fail to writing\n");
        return -1;
    }
    char temp[50];
    int i =0;
    while(fgets(temp,50,p)!=NULL){
        temp[strlen(temp)-1]='\0';
        if (validate(temp) == 0){
            continue;
        }
        printf("%s\n",temp );
        fprintf(q,"%s\n",temp);   
        i++;
    }
    fclose(p); 
    fclose(q);
    printf("====================\n");
    printf("so luong : %d\n",i);
    return 0;
}

// #include <stdio.h>
// #include <string.h>
// int main(){
//     // char c = 'è';
//     // char d = 'o';
//     // int res = strcmp(d,c);
//     printf("result  : %c\n",133);
// }