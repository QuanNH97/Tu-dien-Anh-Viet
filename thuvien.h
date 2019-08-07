#include "thuvien2.h"
#define MAX_WORD 40
#define MAX_DATA 1000000


GdkPixbuf *create_pixbuf(const gchar * filename) {
    
   GdkPixbuf *pixbuf;
   GError *error = NULL;
   pixbuf = gdk_pixbuf_new_from_file(filename, &error);
   
   if (!pixbuf) {
       
      fprintf(stderr, "%s\n", error->message);
      g_error_free(error);
   }

   return pixbuf;
}

//docfile

void creat_data(BTA **tudien,BTA **soundex,char *filename)
{
    *tudien = btcrt("data",0,0);
    *soundex = btcrt("Cay_soundex",0,0);
    char id_soundex[5];
    FILE *f;
    if(!(f=fopen(filename,"r")))
    {
        printf("Mở file không thành công\n");
        return ;
    }
    char word[MAX_WORD],data[MAX_DATA],tmp[100];
    data[0]='\0';
    while(!feof(f))
    {
        fgets(tmp,100,f);
        while(tmp[0]!='@')
        {
            strcat(data,tmp);
            if(feof(f)) break;
            fgets(tmp,100,f);
        }
        word[strlen(word)-1]='\0';
        //printf("Word %s\nData:%s\n",word,data);
        btins(*tudien,word,data,500*sizeof(char));
        SoundEx(id_soundex,word,4,1);
        btins(*soundex,word,id_soundex,5*sizeof(char));
        if(tmp[0]=='@')
        {
            strcpy(word,tmp);
            int i,j,k=0,check=0;
            for(i=0;i<strlen(word);i++)
            {
                if(word[i]=='/')
                    break;
            }
            for(j=i;j<=strlen(word);j++)
            {
                data[k]=word[j];
                k++;
            }
            word[i-1]='\0';
            for (int i = 1; i < strlen(word); ++i)
                word[i-1]=word[i];
        }

    }
  
}

void tree_gtk(int argc, char **argv)
{
    GdkPixbuf *icon;
    GtkWidget *window;
    GtkWidget *layout,*layout1;
    GtkWidget *layoutbox;
    GtkWidget *searchButton;
    GtkWidget *label1,*label2,*label3,*label4,*label5;
    GtkWidget *notebook;
    GtkWidget *tieude;
    GtkWidget *notetext;

	

    char a[25][100]={"\0","\0","\0","\0""\0","\0","\0","\0","\0","\0""\0","\0"};

    gtk_init (&argc, &argv);

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (window), "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\tENGLISH TRANSLATE");
    gtk_window_set_default_size (GTK_WINDOW (window), 1200,800);
    g_signal_connect (window, "delete_event", gtk_main_quit, NULL); 
    icon = create_pixbuf("icon.jpg");  
    gtk_window_set_icon(GTK_WINDOW(window), icon);

    layout=gtk_layout_new(NULL,NULL);
    gtk_container_add(GTK_CONTAINER(window), layout);
    layoutbox = xpm_label_box2 ("hinhnen.jpg");
    gtk_container_add (GTK_CONTAINER (layout), layoutbox);
    gtk_widget_show(layout);
    gtk_widget_show (layoutbox);

//GIAO DIEN HIEN THI DU LIEU 

    notebook = gtk_notebook_new ();
    gtk_notebook_set_tab_pos (GTK_NOTEBOOK (notebook), GTK_POS_TOP);
    gtk_widget_show (notebook);

    layout1=gtk_layout_new(NULL,NULL);
    gtk_widget_show(layout1);
    gtk_widget_set_size_request (layout1, 100, 100);

    notetext=gtk_text_view_new();
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(notetext));
    gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);

    gtk_layout_put (GTK_LAYOUT(layout1),notetext,0,0);
    label = gtk_label_new ("Nghĩa :");
    gtk_notebook_append_page (GTK_NOTEBOOK (notebook), layout1, label);
    
    gtk_notebook_set_current_page (GTK_NOTEBOOK (notebook), 0);
    gtk_widget_set_size_request(notebook,500,300);

    gtk_layout_put (GTK_LAYOUT(layout),notebook,400,200);

//Giao dien thong bao

    label_thong_bao = gtk_label_new("Thông Báo :");
    gtk_layout_put(GTK_LAYOUT(layout),label_thong_bao,400,515);

    thong_bao = gtk_entry_new();
    gtk_widget_set_size_request(thong_bao,300,20);
    gtk_layout_put(GTK_LAYOUT(layout),thong_bao,530,515);

// Giao dien tim kiem

    searchButton = gtk_button_new_with_label ("Tìm kiếm");
    g_signal_connect (searchButton, "clicked",G_CALLBACK (search),notebook);
    gtk_layout_put (GTK_LAYOUT(layout),searchButton,1000,200);

    searchEntry=gtk_search_entry_new ();
    gtk_widget_set_size_request(searchEntry,500,20);
    //g_signal_connect ((gpointer) searchEntry, "focus",G_CALLBACK (enter_callback2),notebook);
    g_signal_connect(searchEntry, "key-press-event", G_CALLBACK(hoan_thanh), notebook);
    g_signal_connect (searchEntry, "search-changed",G_CALLBACK (goi_y),notebook);
    g_signal_connect (searchEntry, "activate",G_CALLBACK (search),notebook);
    gtk_layout_put (GTK_LAYOUT(layout),searchEntry,400,150);


//Giao dien them


    them_word_Entry = gtk_entry_new();
    gtk_widget_set_size_request(them_word_Entry,150,20);
    gtk_layout_put(GTK_LAYOUT(layout),them_word_Entry,450,560);

    them_nghia_Entry = gtk_entry_new();
    gtk_widget_set_size_request(them_nghia_Entry,100,20);
    gtk_layout_put(GTK_LAYOUT(layout),them_nghia_Entry,740,560);

    addButton = gtk_button_new_with_label("   Thêm  ");
    g_signal_connect (addButton, "clicked",G_CALLBACK (add),NULL);
    gtk_layout_put(GTK_LAYOUT(layout),addButton,930,560);


    label1 = gtk_label_new("Từ:");
    gtk_layout_put(GTK_LAYOUT(layout),label1,400,565);

    label2 = gtk_label_new("Nghĩa :");
    gtk_layout_put(GTK_LAYOUT(layout),label2,670,565);

//Giao dien sua
    edit_word_Entry = gtk_entry_new();
    gtk_widget_set_size_request(edit_word_Entry,150,20);
    gtk_layout_put(GTK_LAYOUT(layout),edit_word_Entry,450,600);

    edit_nghia_Entry = gtk_entry_new();
    gtk_widget_set_size_request(edit_nghia_Entry,100,20);
    gtk_layout_put(GTK_LAYOUT(layout),edit_nghia_Entry,740,600);

    edit_Button = gtk_button_new_with_label("    Sửa      ");
    g_signal_connect (edit_Button, "clicked",G_CALLBACK (edit),NULL);
    gtk_layout_put(GTK_LAYOUT(layout),edit_Button,930,600);


    label4 = gtk_label_new("Từ:");
    gtk_layout_put(GTK_LAYOUT(layout),label4,400,605);

    label5 = gtk_label_new("Nghĩa :");
    gtk_layout_put(GTK_LAYOUT(layout),label5,670,605);


//Giao dien xoa

    label3 = gtk_label_new("Từ:");
    gtk_layout_put(GTK_LAYOUT(layout),label3,400,655);

    deleteSearchEntry = gtk_entry_new();
    gtk_widget_set_size_request(deleteSearchEntry,450,20);
    gtk_layout_put(GTK_LAYOUT(layout),deleteSearchEntry,450,650);


    deleteSearchButton = gtk_button_new_with_label("     Xóa    ");
    g_signal_connect (deleteSearchButton, "clicked",G_CALLBACK (deleted),notebook);
    gtk_layout_put(GTK_LAYOUT(layout),deleteSearchButton,930,650);

//GIAO DIEN SOUDEX

    view = create_view_and_model (a,20);// 20 so ptu goi y
    gtk_widget_set_size_request(view,270,250); // kich thuoc khung
    gtk_container_set_border_width (GTK_CONTAINER (view), 12);
    gtk_layout_put (GTK_LAYOUT(layout),view,50,200);//toa do
    



//khoi tao
    gtk_widget_show_all (window);

    btinit();
    root=btopn("data",0,0);
    soundexTree=btopn("Cay_soundex",0,0);
    if(!root && ! soundexTree){
        printf("\t\tData Loading.... \n");
        creat_data(&root,&soundexTree,"anhviet.txt");
    }
    g_object_unref(icon);  
    gtk_main ();


   
}
