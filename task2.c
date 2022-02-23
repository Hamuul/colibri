#include <stdio.h>
#include <string.h>
#include "site.h"

int cmp(const void *item1, const void *item2)
// functia comparator alfabetic dupa continut, sau eventual dupa accesari
{
    site *site1 = (site*) item1;
    site *site2 = (site*) item2;
    int rez = strcmp(site1->continut, site2->continut);
    return (rez != 0)?(rez):(site1->no_acc - site2->no_acc);
}

int main(void)
{
    siteVector arr_sites = citire_date();
    int i;
    for(i = 0; i < arr_sites.used_len; i++)
    {
        char *aux = strdup(extract_p_text(arr_sites.sites[i].continut));
        strcpy(arr_sites.sites[i].continut, aux);
        free(aux);
    }
    qsort(arr_sites.sites, arr_sites.used_len, sizeof(site), *(cmp));
    char search_terms[128];
    fgets(search_terms, 128, stdin);
    char *p = strtok(search_terms, " \n");
    char *search_words[8];
    int cnt = 0;
    char afisate[10];
    for(i = 0; i < 10; i++)
    {
        afisate[i] = '0';
    }
    while(p != NULL)
    {
        search_words[cnt++] = strdup(p);
        p = strtok(NULL, " \n");
    }
    for(i = 0; i < arr_sites.used_len; i++)
    {
        p = strtok(arr_sites.sites[i].continut, " \n?-!,");
        while(p != NULL)
        {
            int j;
            for(j = 0; j < cnt; j++)
            {
                if(strcmp(search_words[j], p) == 0 && afisate[i] == '0')
                {
                    printf("%s\n", arr_sites.sites[i].url);
                    afisate[i] = '1';
                }
            }
            p = strtok(NULL, " \n?-!,");
        }
    }
    for(i = 0; i < cnt; i++)
    {
        free(search_words[i]);
    }
    free_site_vector(&arr_sites);
    return 0;
}