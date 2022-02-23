#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned long long llu;
typedef unsigned int uint;

typedef struct
{
    llu no_acc;
    char *continut;
    char url[50];
    char title[50];
    int checksum;
} site;

typedef struct
{
    site *sites;
    int cap;
    int used_len;
} siteVector;

typedef struct
{
    char filename[31];
} siteFiles;

#define trim(str) do{\
    int i;\
    int begin = 0;\
    int end = strlen(str) - 1;\
    while (str[begin] == ' ' || str[begin] == '\n')\
        begin++;\
    while ((end >= begin) && (str[end] == ' ' || str[end] == '\n'))\
        end--;\
    for (i = begin; i <= end; i++)\
        str[i - begin] = str[i];\
    str[i - begin] = '\0';\
}while(0)

siteVector citire_date();
void init_site_vector(siteVector *vecptr);
void free_site_vector(siteVector *vecptr);
void extract_title(char *continut, char *title);
void add_site(siteVector *vecptr, site *site_to_add);

void init_site_vector(siteVector *vecptr)
// initializeza vectorul de site-uri cu dimensiunea maxima de 3 site-uri
{
    vecptr->sites = (site *)malloc(sizeof(site) * 3);
    if(vecptr == NULL)
    {
        printf("memory allocation failed\n");
    }
    else
    {
        vecptr->cap = 3;
        vecptr->used_len = 0;
    }
}

void free_site_vector(siteVector *vecptr)
// elibereaza memoria alocata dinamic in ordinea inversa a alocarii
{
    int i;
    for(i = 0; i < vecptr->used_len; i++)
    {
        free(vecptr->sites[i].continut);
        vecptr->sites[i].continut = NULL;
    }
    free(vecptr->sites);
    vecptr->sites = NULL;
}

// adauga un site in vectorul de siteuri, direct daca mai exista capacitate,
// altfel extinde capacitatea vectorului cu inca 3
#define add_site(vecptr,site_to_add) do{\
    vecptr = (siteVector*) vecptr;\
    if(vecptr->used_len < vecptr->cap)\
    {\
        vecptr->sites[vecptr->used_len] = *site_to_add;\
        vecptr->used_len += 1;\
    }\
    else\
    {\
        vecptr->cap += 3;\
        vecptr->sites = (site*)realloc\
                           (vecptr->sites, vecptr->cap * sizeof(site));\
        if(vecptr-> sites == NULL)\
        {\
            printf("allocation failed\n");\
        }\
        else\
        {\
            vecptr->sites[vecptr->used_len] = *site_to_add;\
            vecptr->used_len += 1;\
        }\
    }\
}while(0)

siteVector citire_date(void)
// deschide master.txt, stocheaza numele fisierelor html, deschide fiecare .html
// apoi parseaza continutul sau intr-un siteVector
{
    siteVector arr_sites;
    init_site_vector(&arr_sites);
    site my_site;
    FILE *fp_master = fopen("master.txt", "r");
    if(fp_master != NULL)
    {
        char buf[101];
        int cnt = 0;
        siteFiles *my_files;
        while(fgets(buf, 81, fp_master) != NULL)
        {
            cnt++;
        };
        my_files = (siteFiles *)malloc(cnt * sizeof(siteFiles));
        cnt = 0;
        rewind(fp_master);
        if(my_files != NULL)
        {
            while(fgets(my_files[cnt].filename, 31, fp_master) != NULL)
            {
                trim(my_files[cnt].filename);
                cnt++;
            }
        }
        else
        {
            printf("allocation failed\n");
        }
        int i;
        FILE *fp;
        for(i = 0; i < cnt; i++)
        {
            fp = fopen(my_files[i].filename, "r");
            if(fp != NULL)
            {
                char url[50];
                int checksum;
                uint no_bytes;
                llu no_acc;
                fgets(buf, 101, fp);
                sscanf(buf, "%50s%u%llu%d", url, &no_bytes, &no_acc, &checksum);
                strcpy(my_site.url, url);
                my_site.no_acc = no_acc;
                my_site.checksum = checksum;
                my_site.continut = (char *) calloc(no_bytes + 1, 1);
                if(my_site.continut != NULL)
                {
                    fread(my_site.continut, 1, no_bytes, fp);
                    my_site.continut[no_bytes] = '\0';
                    char *aux = strdup(my_site.continut);
                    extract_title(aux, my_site.title);
                    trim(my_site.title);
                    free(aux);
                }
                else
                {
                    printf("memory allocation for continut failed\n");
                }
                siteVector *adr = &arr_sites;
                site *to_add = &my_site;
                add_site(adr, to_add);
            }
            else
            {
                puts(my_files[i].filename);
                printf("failed to open site txt files\n");
            }
            fclose(fp);
        }
        fclose(fp_master);
        free(my_files);
    }
    else
    {
        printf("failed to open masters.txt\n");
    }
    return arr_sites;
}


void extract_title(char *continut, char *title)
// extrage titlul din codul HTML
{
    char *start = strstr(continut, "<title>");
    char *ptr = strstr(start, ">");
    char *ptr2 = strstr(ptr, "<");
    ptr2[0] = '\0';
    int i = 0;
    while(*ptr++ != '\0')
    {
        title[i] = *ptr;
        i++;
    }
    title[i] = '\0';
}

char *extract_p_text(char *continut)
// extrage ce e in paragrafe din codul HTML si scoate spatii leading si trailing
{
    char *start = strstr(continut, "<p");
    char *ptr = strstr(start, ">") + 1;
    char *ptr2 = strstr(ptr, "<");
    *ptr2 = '\0';
    trim(ptr);
    return ptr;
}
