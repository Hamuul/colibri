#include <stdio.h>
#include <string.h>
#include "site.h"

int cmp(const void *item1, const void *item2)
//functie comparator dupa nr. de accesari
{
    site *site1 = (site*) item1;
    site *site2 = (site*) item2;
    return (site2->no_acc - site1->no_acc);
}

int main(void)
{
    siteVector arr_sites = citire_date();
    char search_terms[128], copy[128];
    fgets(search_terms, 128, stdin);
    strcpy(copy, search_terms);
    int i = 0;
    char *p1, *whole = NULL;
    if(strchr(copy, '"') != NULL)
    {
        p1 = strchr(copy, '"') + 1;
        char *p2 = strrchr(copy, '"');
        whole = (char*) calloc(p2 - p1 + 1, 1);
        if(whole != NULL)
        {
            memcpy(whole, p1, p2 - p1);
        }
    }
    for(i = 0; i < arr_sites.used_len; i++)
    {
        char *aux = strdup(extract_p_text(arr_sites.sites[i].continut));
        strcpy(arr_sites.sites[i].continut, aux);
        free(aux);
    }
    qsort(arr_sites.sites, arr_sites.used_len, sizeof(site), *(cmp));
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
        if(p[0] != '"' && p[strlen(p)-1] != '"' &&
                (whole == NULL || strstr(whole, p) == NULL))
        {
            search_words[cnt++] = strdup(p);
        }
        p = strtok(NULL, " \n");
    }
    for(i = 0; i < arr_sites.used_len; i++)
    {
        p = strtok(arr_sites.sites[i].continut, " \n?-!.,");
        while(p != NULL)
        {
            int j;
            for(j = 0; j < cnt; j++)
            {
                if(search_words[j][0] == '-' &&
                    strcmp(search_words[j] + 1, p) == 0)
                {
                    afisate[i] = 'x';
                }
            }
            p = strtok(NULL, " \n?-!.,");
        }
    }
    free_site_vector(&arr_sites);
    arr_sites = citire_date();
    for(i = 0; i < arr_sites.used_len; i++)
    {
        char *aux = strdup(extract_p_text(arr_sites.sites[i].continut));
        strcpy(arr_sites.sites[i].continut, aux);
        free(aux);
    }
    qsort(arr_sites.sites, arr_sites.used_len, sizeof(site), *(cmp));
    for(i = 0; i < arr_sites.used_len; i++)
    {
        if(whole != NULL && strstr(arr_sites.sites[i].continut, whole) != NULL
            && afisate[i] == '0')
        {
            printf("%s\n", arr_sites.sites[i].url);
            afisate[i] = '1';
        }
        p = strtok(arr_sites.sites[i].continut, " \n?-!.,");
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
            p = strtok(NULL, " \n?-!.,");
        }
    }
    for(i = 0; i < cnt; i++)
    {
        free(search_words[i]);
    }
    free(whole);
    free_site_vector(&arr_sites);
    return 0;
}