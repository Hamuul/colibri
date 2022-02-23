#include <stdio.h>
#include "site.h"

void show_sites_task1(siteVector *vecptr)
// afiseaza linkul, nr. de accesari si titlul cu spatii intre ele si /n la sf.
{
    int i;
    for(i = 0; i < vecptr->used_len; i++)
    {
        printf("%s ", vecptr->sites[i].url);
        printf("%llu ", vecptr->sites[i].no_acc);
        printf("%s\n", vecptr->sites[i].title);
    }
}

int main(void)
{
    siteVector arr_sites = citire_date();
    show_sites_task1(&arr_sites);
    free_site_vector(&arr_sites);
    return 0;
}
