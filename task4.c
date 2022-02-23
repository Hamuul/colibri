#include <stdio.h>
#include <string.h>
#include "site.h"
#include "task4.h"


int main(void)
{
    siteVector arr_sites = citire_date();
    char url[51];
    int i;
    while(fgets(url, 51, stdin) != NULL)
    {
        int found = 0;
        if(url[0] == '\n')
        {
            break;
        }
        url[strlen(url) - 1] = '\0';
        for(i = 0; i < arr_sites.used_len; i++)
        {
            if(strcmp(url, arr_sites.sites[i].url) == 0)
            {
                found = 1;
            }
        }
        if(!found)
        {
            printf("Website not found!\n");
        }
        for(i = 0; i < arr_sites.used_len; i++)
        {
            if(strcmp(url, arr_sites.sites[i].url) == 0)
            {
                int true_hash = hash(arr_sites.sites[i].continut);
                int stated_hash = arr_sites.sites[i].checksum;
                if(true_hash == stated_hash)
                {
                    printf("Website safe!\n");
                }
                else
                {
                    printf("Malicious website! Official key: %d. Found key: %d\n",
                           stated_hash, true_hash);
                }
            }
        }
    }
    free_site_vector(&arr_sites);
    return 0;
}