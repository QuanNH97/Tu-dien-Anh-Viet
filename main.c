
//gcc `pkg-config --cflags gtk+-3.0` -o main main.c `pkg-config --libs gtk+-3.0` libbt.a



#include <gtk/gtk.h>
#include "thuvien.h"
#include <stdlib.h>
int main (int argc, char **argv)
{
  tree_gtk(argc,argv);
  btcls(root);
  btcls(soundexTree);
  return 0;
}


