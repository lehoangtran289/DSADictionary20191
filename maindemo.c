//gcc `pkg-config --cflags gtk+-3.0` -o dictionary maindemo.c `pkg-config --libs gtk+-3.0` -w libbt.a -rdynamic

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "btree.h"
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

typedef struct {
    GtkWidget *searchentry;
    GtkWidget *textview;
    GtkWidget *suggests;
} Widgets;

BTA *dict, *sndex;
Widgets *w;
GtkWidget   *add, *del, *search;
GtkWidget   *thongbao;

void on_add_clicked(GtkButton *button, Widgets *w) {
    dict = btopn("dictionary.db", 0, 0);
    GtkTextIter s, e;
    gchar *input;
    GtkTextBuffer *textview;
    textview = gtk_text_view_get_buffer(GTK_TEXT_VIEW(w->textview));
    input = gtk_entry_get_text(GTK_ENTRY(w->searchentry));

    int value;
    if (bfndky(dict, input, &value)!= 0) {
        binsky(dict, input, 999999);  
        gtk_label_set_text(thongbao, "word added!");
        gtk_text_buffer_set_text(textview, "", -1);
    } else {
        gtk_label_set_text(thongbao, "Word found in dictionary, cant add!");
    }
    //gtk_widget_hide(add);
    btcls(dict);
}

void on_del_clicked(GtkButton *button, Widgets *w) {
    dict = btopn("dictionary.db", 0, 0);
    gchar *input;
    GtkTextBuffer *textview;
    textview = gtk_text_view_get_buffer(GTK_TEXT_VIEW(w->textview));
    input = gtk_entry_get_text(GTK_ENTRY(w->searchentry));

    int value;
    int check = bfndky(dict, input, &value);
    if (check == 0) {
        bdelky(dict, input);
        gtk_entry_set_text(w->searchentry,"");
	    gtk_text_buffer_set_text(textview,"",-1);
	    
    	gtk_label_set_text(thongbao, "Word Deleted!");
    } else {
        gtk_label_set_text(thongbao, "Cant delete. Word doesnt exist!");
        gtk_text_buffer_set_text(textview, "", -1);
    }

 	//gtk_widget_hide(del);
	//gtk_widget_hide(add);
    btcls(dict); 
}

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

void findsug(BTA *dex, char *word, Widgets *w) {
    int size;
    char temp[1000], value[100000];
    char suggests[1000];

    //strcpy(temp, word);
    soundex(word, temp);

    int check = btseln(dex, temp, value, 100000, &size);
    if(check != 0) {
        gtk_entry_set_text(w->suggests, "");
        //return;
    }
    else {
        gtk_entry_set_text(w->suggests, value);
    }
}

// void delchar(char *x, int a, int b){
//     if ((a+b-1) <= strlen(x)) {
//         strcpy(&x[b-1],&x[a+b-1]);
//     }
// }

void cutstring(char input[], char s1[], char s2[]) {
    char delim[] = "\t";
    char s[100];
    strcpy(s2, input);
    char *ptr = strtok(input, delim);

    strcpy(s1, ptr);

    ptr = strtok(NULL, delim);
    strcpy(s2, ptr);
    printf("%s\n\n-s1: %s- s2: %s -\n\n", input, s1, s2);
}

char inp[100000], s1[10000], s2[10000];
gboolean autoComplete(Widgets *widget, GdkEvent *event, gpointer user_data) {
    gchar *suggestion, *entryget;
    GdkEventKey key = event->key;
    
    if(key.keyval == GDK_KEY_Tab) {
        suggestion = gtk_entry_get_text(GTK_ENTRY(w->suggests));
        entryget = gtk_entry_get_text(GTK_ENTRY(widget));

        if(strcmp(suggestion, "") == 0) return FALSE;

        strcpy(inp, suggestion);
        cutstring(inp, s1, s2);
        //printf("\nhere\n");    
        if(strcmp(entryget, s1) != 0) {
            gtk_entry_set_text(widget, s1);
        }
        else {
            gtk_entry_set_text(widget, s2);
            //printf("\nhere2\n");
            findsug(sndex, s2, w);
        }   
        return TRUE;
    }
    return FALSE;
}

void quickSuggest(GtkButton *button, Widgets *w) {
    BTA *dict2 = btopn("dictionary.db", 0, 0);
    sndex = btopn("dictionarySoundex.db", 0, 0);

    gtk_label_set_text(thongbao, "...");

    char suggestword[1000];
    int rsize;

    gchar *input;
    GtkTextBuffer *textview;
    input = gtk_entry_get_text(GTK_ENTRY(w->searchentry));
    textview = gtk_text_view_get_buffer(GTK_TEXT_VIEW(w->textview));
    
    //strcpy(suggestword, input);

    if(strcmp(input, "") == 0) {
        gtk_text_buffer_set_text(textview, "", -1);
        gtk_entry_set_text(w->suggests, "");
    }
    else {
        int index = 1;
        int check = bfndky(dict2, input, &index);
        printf("%s -> %d \n ", input, index);

        if (check != 0) { //suggest when not found
            char soundexCode[100];
            soundex(input, soundexCode); //word -> soundex code
            printf("%s -> %s\n", input, soundexCode);
            
            char value[100000];
            int checksug = btsel(sndex, soundexCode, value, 100000, &rsize);
            if (checksug != 0) {
                gtk_entry_set_text(w->suggests, "");
                return;
            }
            else {
                //strncpy(suggests, value, 10000);
                //printf("%s\n", value);
                gtk_entry_set_text(w->suggests, value);
            }
        } else {
            gtk_entry_set_text(w->suggests, "");
        } 
    }
    btcls(sndex);
    btcls(dict2);
}


void on_searchentry_activate(GtkButton *button, Widgets *w) {
    dict = btopn("dictionary.db", 0, 0);
    int value = 0;

    gchar *input;
    GtkTextBuffer *textview;
    input = gtk_entry_get_text(GTK_ENTRY(w->searchentry));
    textview = gtk_text_view_get_buffer(GTK_TEXT_VIEW(w->textview));

    //gtk_widget_show(add);
    if(strcmp(input,"") == 0) {
        gtk_text_buffer_set_text(textview, "", -1); //delete view
    } 
    else {
        int check = bfndky(dict, input, &value);
        if (check == 0) { //found
            printf("found - index %d\n", value);
            char tmp[100]; 
            sprintf(tmp, "%d", value);
            gtk_text_buffer_set_text(textview, tmp, -1);
            //gtk_widget_show(del);
        }
        else { //not found
            //gtk_widget_show(add);
            gtk_text_buffer_set_text(textview, "", -1);
            gtk_label_set_text(thongbao, "Word not found! Do you want to add this word?");
        }
    }
    btcls(dict);
}

int main(int argc, char *argv[]) {; 
    // char d[] = "dictionary.db";
    // char s[] = "dictionarySoundex.db";
	btinit();

	// BTA *dict = btopn(d, 0, 0);
    // sndex = btopn(s, 0, 0);
    GtkWidget       *window;
    GtkBuilder      *builder;
    GError          *error = NULL;
    w = g_slice_new(Widgets);

    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("dictionary.glade");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    
    search = GTK_WIDGET (gtk_builder_get_object (builder, "search"));

    add = GTK_WIDGET (gtk_builder_get_object (builder, "add"));
    del = GTK_WIDGET (gtk_builder_get_object (builder, "del"));
    thongbao = GTK_WIDGET (gtk_builder_get_object (builder, "thongbao"));

    w->searchentry = GTK_WIDGET( gtk_builder_get_object( builder, "searchentry" ) );
    w->textview = GTK_WIDGET( gtk_builder_get_object( builder, "textview" ) );
    w->suggests = GTK_WIDGET( gtk_builder_get_object( builder, "suggests" ) );

    gtk_builder_connect_signals (builder, w);
    g_object_unref (G_OBJECT (builder));
        
    gtk_widget_show (window);   

    gtk_main ();

    //btcls(dict);
    //btcls(sndex);
    printf("Closed!\n");
    return 0;
}