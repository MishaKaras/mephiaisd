#include "h_table.h"
#ifndef DIALOGS_H
#define DIALOGS_H
void errors(int);
int D_insert(Table *);
int D_delete(Table *);
int D_delete_r(Table *);
int D_find(Table *);
int D_find_r(Table *);
int D_output(Table *);
int D_import(Table *);
int D_export(Table *);
#endif
