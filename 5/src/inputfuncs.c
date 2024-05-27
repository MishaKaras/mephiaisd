#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inputfuncs.h"

int get_int(int* perem)
{
    int stat = 0;
    char lc = '_';
    stat = scanf("%d%c", perem, &lc);

    while (stat != 2 || lc != '\n')
    {
        if (stat == -1)
            break;
        if (lc != '\n' || stat == 0)
            scanf("%*[^\n]%*c");
        printf("Введите еще раз: ");
        stat = scanf("%d%c", perem, &lc);
    }
    return stat;
}

int get_len(size_t* res)
{
    int stat = 0;
    int perem;
    char lc = '_';
    stat = scanf("%d%c", &perem, &lc);

    while (stat != 2 || lc != '\n' || perem <= 0)
    {
        if (stat == -1)
            break;
        if (lc != '\n' || stat == 0)
            scanf("%*[^\n]%*c");
        printf("Введите еще раз: ");
        stat = scanf("%d%c", &perem, &lc);
    }
    *res = (size_t)perem;
    return stat;
}


int get_double(double* perem)
{
    int stat = 0;
    char lc = '_';
    stat = scanf("%lf%c", perem, &lc);

    while (stat != 2 || lc != '\n')
    {
        if (stat == -1)
            break;
        if (lc != '\n' || stat == 0)
            scanf("%*[^\n]%*c");
        printf("Введите еще раз: ");
        stat = scanf("%lf%c", perem, &lc);
    }
    return stat;
}

char *txt_readline(FILE *fptr)
{
	char *buff = (char*)calloc(6, sizeof(char));
	char *res = (char*)calloc(1, sizeof(char));
	int reslen = 1, bufflen = 0;
    int stat = fscanf(fptr, "%5[^\n]", buff);
    while (stat != 0)
    {
    	if (stat == EOF)
    	{
    		free(res);
    		if (buff != NULL)
    			free(buff);
    		return NULL;
    	}
        bufflen = strlen(buff);
        char *newres = (char*)realloc(res, (reslen + bufflen) * sizeof(char));
        if (newres == NULL)
        {
            printf("Ошибка выделения памяти.\n");
            free(res);
            return NULL;
        }
        else if (res != newres) 
            res = newres;
        memcpy(res + reslen - 1, buff, bufflen * sizeof(char));
        reslen += bufflen;
        res[reslen-1] = '\0';
        if (feof(fptr))
            break;
        stat = fscanf(fptr, "%5[^\n]", buff);
    }
    fscanf(fptr, "%*c");
    if (buff != NULL)
        free(buff);
    return res;
}
