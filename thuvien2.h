#include "btree.h"
#include "soundex.h"
#include <string.h>
#include <gdk/gdkkeysyms.h>
GtkTreeModel *model;
enum
{
	COL_NAME = 0,
	NUM_COLS
} ;


static GtkTreeModel * create_and_fill_model (char a[][100],int k)
{ GtkListStore  *store;
  GtkTreeIter    iter;
  store = gtk_list_store_new (NUM_COLS, G_TYPE_STRING, G_TYPE_UINT);// loai store
  int i=0;
  for(i=0;i<k;i++){  // tao k dong moi
   gtk_list_store_append (store, &iter);// noi dong
   gtk_list_store_set (store, &iter,COL_NAME, a[i],-1);// doc a[] vao cac dong
  }
   return GTK_TREE_MODEL (store);
}
static GtkWidget *create_view_and_model (char a[][100],int k)
{ GtkCellRenderer     *renderer;
  GtkWidget           *view;
  view = gtk_tree_view_new ();
  renderer = gtk_cell_renderer_text_new ();
  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),-1,"Gợi ý:",renderer,"text", COL_NAME,NULL);
  model = create_and_fill_model (a,k);// tao model 
  gtk_tree_view_set_model (GTK_TREE_VIEW (view), model);// hien thi model ra man hinh
  g_object_unref (model);
  return view;
}


GtkWidget *searchEntry;
GtkWidget *label;
GtkWidget *view,*viewls;

GtkWidget *deleteSearchEntry;
GtkWidget *deleteSearchButton;

GtkWidget *them_word_Entry;
GtkWidget *them_nghia_Entry;
GtkWidget *addButton;

GtkWidget *edit_word_Entry;
GtkWidget *edit_nghia_Entry;
GtkWidget *edit_Button;

GtkWidget *label_thong_bao;
GtkWidget *thong_bao;


char word[1000],data[1000];
GtkTextBuffer *buffer;
GtkTextIter iter;

BTA *root,*soundexTree;

gboolean hoan_thanh(GtkWidget *widget,GdkEventKey *event,GtkNotebook *notebook)
{
 
 	gint page;
	page = gtk_notebook_get_current_page (notebook);
	gtk_notebook_set_current_page (GTK_NOTEBOOK (notebook), 0);
	char *e;
	e=(char*) gtk_entry_get_text (GTK_ENTRY(searchEntry));
	if(strcmp(gdk_keyval_name(event->keyval),"Tab")!=0)
		return FALSE;	
	int len = strlen(e);
	int size;
	char k[40];
	if(btsel(root,e,data,len*sizeof(char),&size)!=0)
	{
		btins(root,e,"",len*sizeof(char));
		btsel(root,e,data,len*sizeof(char),&size);
		btseln(root,k,data,len*sizeof(char),&size);
		gtk_entry_set_text (GTK_ENTRY(searchEntry),k);
		btselp(root,k,data,len*sizeof(char),&size);
		btdel(root,k);
	}
	return 1;

}

//tra lai so tu cung ma soundex
int suggestion(BTA *soundexTree, char *word, char s[][100]){//tra ve mang s co chua ma soundex vs i phan tu t/m
	int rsize,i=0;
	char en[40],soundex[5],code[5];
	SoundEx(code,word,4,1);// tao ma soundex cua word tra ve cho code
	btsel(soundexTree,"",soundex,5*sizeof(char),&rsize); // duyet cay soundex key=tu val=soundex
	while(btseln(soundexTree,en,soundex,5*sizeof(char),&rsize)==0 && i<20){
		if(strcmp(code,soundex)==0) // neu cung ma soundex -> copy vao xau s[][]
			strcpy(s[i++],en);
                      //  printf("%s\n",s[i-1]);
	}
	return i;//so ptu mang s[] chua ma soundex
}

void goi_y(GtkButton  *button,GtkNotebook *notebook )
{
	char suggest[100][100];
	char *e;
	int k,i;
	e=(char*) gtk_entry_get_text (GTK_ENTRY (searchEntry));// lay text 
	k=suggestion(soundexTree,e,suggest);//so luong cung ma soundex + copy soundex vao mang suggest[][]
        // printf("%d \n",k);
	model = create_and_fill_model (suggest,k);// tao ...
	if(k!=0)gtk_tree_view_set_model (GTK_TREE_VIEW (view), model);//ghi du lieu vao khung soundex
}
// button search
void search( GtkButton *button,GtkNotebook *notebook )
{
	gint page;
	page = gtk_notebook_get_current_page (notebook);
	gtk_notebook_set_current_page (GTK_NOTEBOOK (notebook), 0);
	char *e;
	e=(char*) gtk_entry_get_text (GTK_ENTRY(searchEntry));
	int j;
	int k=btsel(root,e,data,sizeof(data),&j);
	if(k==0) {/*strcat(data,them);*/gtk_text_buffer_set_text (buffer,data,-1);}
	else gtk_text_buffer_set_text (buffer,"\n\n\n\n\n\n\t\t\t\t\t\tTừ này không tồn tại....",-1);
}

//ButtonDelete

 void deleted( GtkButton   *button,GtkNotebook *notebook )
{

	char *e;
	e=(char*) gtk_entry_get_text (GTK_ENTRY (deleteSearchEntry));
	btdel(soundexTree,e);
	int i=btdel(root,e);

	if(i!=0)gtk_entry_set_text (GTK_ENTRY(thong_bao),"\t\t\t\tXóa thất bại ");
	else gtk_entry_set_text (GTK_ENTRY(thong_bao),"\t\t\t\tXóa thành công");
	gtk_entry_set_text (GTK_ENTRY(deleteSearchEntry),"");

}

//Button add

void add( GtkButton   *button, GtkNotebook *notebook )
{	
	char *e,*f;
	e=(char*) gtk_entry_get_text (GTK_ENTRY (them_word_Entry));
	f=(char*) gtk_entry_get_text (GTK_ENTRY (them_nghia_Entry));
	if(!strcmp(e," ")||!strcmp(e,"  ")||!strcmp(e,"   ")||!strcmp(e,"    ")){
		gtk_entry_set_text (GTK_ENTRY(thong_bao),"\t\t\t\tko the them");
               
	}else{
        
	int i=btins(root,e,f,500*sizeof(char));
	if(i!=0) gtk_entry_set_text (GTK_ENTRY(thong_bao),"\t\t\t\tĐã tồn tại");
	else if(i==0)
	{
		gtk_entry_set_text (GTK_ENTRY(thong_bao),"\t\t\t\tThêm thành công");
		char ma[5];
		SoundEx(ma,e,4,1);
		btins(soundexTree,e,ma,5*sizeof(char));
	}}
	gtk_entry_set_text (GTK_ENTRY(them_word_Entry),"");
	gtk_entry_set_text (GTK_ENTRY(them_nghia_Entry),"");

}

//HAM SUA 
void edit( GtkButton   *button, GtkNotebook *notebook )
{
	char *e,*f,*f1;
	e=(char*) gtk_entry_get_text (GTK_ENTRY (edit_word_Entry));
	f=(char*) gtk_entry_get_text (GTK_ENTRY (edit_nghia_Entry));

	
	if(!btupd(root,e,f,500*sizeof(char)))
		gtk_entry_set_text (GTK_ENTRY(thong_bao),"\t\t\t\tSửa thành công");
	else
		gtk_entry_set_text (GTK_ENTRY(thong_bao),"\t\t\t\tSửa thất bại");


	gtk_entry_set_text (GTK_ENTRY(edit_word_Entry),"");
	gtk_entry_set_text (GTK_ENTRY(edit_nghia_Entry),"");
}


//XU LY GIAO DIEN HINH NEN VA ICON TIEU DE

GtkWidget *xpm_label_box2( gchar     *xpm_filename
	)
{
	GtkWidget *box;
	GtkWidget *image;

    /* Create box for image and label */
	box = gtk_box_new (FALSE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (box), 2);

    /* Now on to the image stuff */
	image = gtk_image_new_from_file (xpm_filename);

    /* Pack the image and label into the box */
	gtk_box_pack_start (GTK_BOX (box), image, FALSE, FALSE, 3);

	gtk_widget_show (image);

	return box;
}
