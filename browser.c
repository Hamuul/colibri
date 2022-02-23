#include <curses.h>
#include <stdlib.h>
#include "site.h"

int cmp_S(const void *item1, const void *item2)
{
    site *site1 = (site*) item1;
    site *site2 = (site*) item2;
    int rez = strcmp(site1->continut, site2->continut);
    return (rez != 0)?(rez):(site1->no_acc - site2->no_acc);
}

int cmp_A(const void *item1, const void *item2)
{
    site *site1 = (site*) item1;
    site *site2 = (site*) item2;
    return (site2->no_acc - site1->no_acc);
}

int main(void)
{
  initscr();
  cbreak();
  char c;
  WINDOW *key = newwin(30, 30, 20, 30);
  wprintw(key, "Colibri Browser\nPress C to open search box and Enter to lock it\nPress S for simple search\nPress A for advanced search\nPress B to search again\nPress Q to exit");
  wrefresh(key);
  while((c = getchar()) != 'Q')
  {
    char query[81];
    if(c == 'C' || c == 'B')
    {
      WINDOW *win = newwin(5, 70, 2, 2);
      wmove(win, 2, 2);
      box(win, 0, 0);
      wrefresh(win);
      wgetnstr(win, query, 80);
    }
    if(c == 'S')
    {
      siteVector arr_sites = citire_date();
      int i;
      for(i = 0; i < arr_sites.used_len; i++)
      {
          char *aux = strdup(extract_p_text(arr_sites.sites[i].continut));
          strcpy(arr_sites.sites[i].continut, aux);
          free(aux);
      }
      qsort(arr_sites.sites, arr_sites.used_len, sizeof(site), *(cmp_S));
      char *p = strtok(query, " \n");
      char *search_words[8];
      int cnt = 0;
      char afisate[10];
      WINDOW *win = newwin(10, 70, 2, 2);
      move(0, 0);
      wprintw(win, "%s\n", query);
      move(2, 2);
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
                      wprintw(win, "%s %s\n", arr_sites.sites[i].url, arr_sites.sites[i].title);
                      wrefresh(win);
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
      wrefresh(win);
      free_site_vector(&arr_sites);
    }
    if(c == 'A')
    {
        siteVector arr_sites = citire_date();
        char copy[81];
        strcpy(copy, query);
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
        qsort(arr_sites.sites, arr_sites.used_len, sizeof(site), *(cmp_A));
        char *p = strtok(query, " \n");
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
                    if(search_words[j][0] == '-' && strcmp(search_words[j] + 1, p) == 0)
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
        WINDOW *win = newwin(5, 70, 2, 2);
        move(0, 0);
        wprintw(win, "%s\n", query);
        move(2, 2);
        qsort(arr_sites.sites, arr_sites.used_len, sizeof(site), *(cmp_A    ));
        for(i = 0; i < arr_sites.used_len; i++)
        {
            if(whole != NULL && strstr(arr_sites.sites[i].continut, whole) != NULL
                && afisate[i] == '0')
            {
                wprintw(win, "%s %s\n", arr_sites.sites[i].url, arr_sites.sites[i].title);
                wrefresh(win);
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
                        wprintw(win, "%s %s\n", arr_sites.sites[i].url, arr_sites.sites[i].title);
                        wrefresh(win);
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
        wrefresh(win);
        free(whole);
        free_site_vector(&arr_sites);
    }
  }
  endwin();
  return 0;
}