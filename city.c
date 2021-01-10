#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

typedef struct city_t {
    int meteo_id;
    int city_id;
    char name[100];  // no city should have
                     // longer than 100 chars
} city_t;


// takes 2 params, the filename and a pointer
// to size_t where the number of cities is stored
city_t *read_cities(const char *filename, size_t *len)
{
    if(filename == NULL || len == NULL)
        return NULL;

    FILE *fp = fopen(filename, "r");
    if(fp == NULL)
    {
        fprintf(stderr, "Could not open %s: %s\n", filename, strerror(errno));
        return NULL;
    }

    city_t *arr = NULL, *tmp;
    *len = 0;

    // assuming that no line will be longer than 1023 chars long
    char line[1024];

    while(fgets(line, sizeof line, fp))
    {
        tmp = realloc(arr, (*len + 1) * sizeof *arr);
        if(tmp == NULL)
        {
            fprintf(stderr, "could not parse the whole file %s\n", filename);
            // returning all parsed cities so far

            if(*len == 0)
            {
                free(arr);
                arr = NULL;
            }

            return arr;
        }

        arr = tmp;

        // %99[^\n] is to read up to 99 characters until the end of the line
        if(sscanf(line, "%d,%d,%99[^\n]", &(arr[*len].meteo_id),
                    &(arr[*len].city_id), arr[*len].name) != 3)
        {
            fprintf(stderr, "Invalid line format (skipping line):\n%s\n", line);
            // skip this line, and decrement *len
            (*len)--;
            continue;
        }

        // incrementing only when parsing of line was OK
        (*len)++;
    }

    fclose(fp);

    // file is empty or
    // all lines have wrong format
    if(*len == 0)
    {
        free(arr);
        arr = NULL;
    }


    return arr;
}

void print_cities(city_t *cities, size_t len, FILE *fp)
{
    if(cities == NULL || fp == NULL)
        return;

    for(size_t i = 0; i < len; ++i)
        fprintf(fp, "%d,%d,%s\n", cities[i].meteo_id, cities[i].city_id,
                cities[i].name);
}

int search_for_city_by_name(city_t *cities, size_t len, const char *name)
{
    if(cities == NULL || name == NULL)
        return -1;

    for(size_t i = 0; i < len; ++i)
        if(strcmp(name, cities[i].name) == 0)
            return i;

    // not found
    return -1;
}

int main(void)
{
    size_t cities_len;
    city_t *cities = read_cities("city.csv", &cities_len);

    // error
    if(cities == NULL)
        return 1;

    city_t ola;
    ola.city_id=1234,
    ola.meteo_id=12345;
    strcpy(ola.name,"ika");

    cities[cities_len++]=ola;
    
  
    // int c = search_for_city_by_name(cities, cities_len, "manhattan");
    // city_t l = cities[c];
    // printf("id: %d",l.city_id);

   // do_something_with_cities(cities, cities_len);

    // update csv
    FILE *fp = fopen("city.csv", "w");

    if(fp == NULL)
    {
        fprintf(stderr, "Could not open city.csv for reading: %s\n",
                strerror(errno));

        free(cities);
        return 1;
    }

    print_cities(cities, cities_len, fp);

    fclose(fp);
    free(cities);
    return 0;
}