#include <stdio.h>
#include <stdlib.h>
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

int get_len(int* perem)
{
    int stat = 0;
    char lc = '_';
    stat = scanf("%d%c", perem, &lc);

    while (stat != 2 || lc != '\n' || *perem <= 0)
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
