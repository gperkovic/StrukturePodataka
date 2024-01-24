#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>
#define N 11

struct City;
typedef struct City* cityPosition;
typedef struct City
{
    char name[20];
    int residents;
    cityPosition right;
    cityPosition left;
}city;

struct Country;
typedef struct Country* countryPosition;
typedef struct Country
{
    char name[20];
    cityPosition city;
    countryPosition next;
}country;

int addCountry(countryPosition countryHead, char* name, char* dat);
int addCity(cityPosition p, char* dat);
cityPosition insertCity(cityPosition p, char* name, int residents);
int countryPrint(countryPosition countryHead);
int cityPrint(cityPosition p);
countryPosition findCountry(countryPosition countryHead, char* name);
int findCity(cityPosition p, int number);

int keyForCountry(char* nameCountry);

int main(void)
{
    country head = { .name = "", .city = NULL, .next = NULL };
    country hash[N];

    FILE* file = NULL;
    char nameCountry[20];
    char datcity[20];
    countryPosition countryHead = NULL;
    int number = 0;
    cityPosition cityHead = NULL;
    int key = 0;
    int i = 0;

    for (i = 0; i < N; i++)
    {
        hash[i].city = NULL;
        hash[i].next = NULL;
        strcpy(hash[i].name, "");
    }

    file = fopen("country.txt", "r");
    if (file == NULL)
    {
        perror("couldn't open file\n");
        return -1;
    }
    while (!feof(file))
    {
        fscanf(file, "%s %s", nameCountry, datcity);
        key = keyForCountry(nameCountry);
        addCountry(&hash[key], nameCountry, datcity);
    }
    fclose(file);

    printf("Country list:\n");
    for (i = 0; i < N; i++)
    {
        //printf("Hash[%d]:\n", i);
        countryPrint(hash[i].next);
    }
    printf("\n\n");

    printf("\nEnter country you want to check: ");
    scanf("%s", nameCountry);
    key = keyForCountry(nameCountry);
    countryHead = findCountry(&hash[key], nameCountry);
    if (countryHead == NULL)
    {
        printf("That country doesn't exist\n");
        return 0;
    }
    printf("Minimal number of residents: ");
    scanf("%d", &number);
    printf("\ncities from that country (%s) with more than %d residents:\n", countryHead->name, number);
    findCity(countryHead->city, number);


    return 0;
}

int addCountry(countryPosition countryHead, char* name, char* dat)
{
    countryPosition newElement = NULL;
    countryPosition previous = NULL;
    cityPosition cityHead = NULL;
    newElement = (countryPosition)malloc(sizeof(country));
    if (newElement == NULL)
    {
        perror("couldn't allocate memory\n");
        return -1;
    }
    strcpy(newElement->name, name);
    newElement->city = cityHead;

    cityHead = (cityPosition)malloc(sizeof(city));
    if (cityHead == NULL)
    {
        perror("couldn't allocate memory\n");
        return -1;
    }
    strcpy(cityHead->name, "");
    cityHead->residents = 0;
    cityHead->right = NULL;
    cityHead->left = NULL;
    newElement->city = cityHead;
    addCity(newElement->city, dat);

    previous = countryHead;
    while (countryHead != NULL)
    {
        if (countryHead->next == NULL)
        {
            countryHead->next = newElement;
            newElement->next = NULL;
            break;
        }
        if (strcmp(newElement->name, previous->next->name) < 0)
        {
            newElement->next = previous->next;
            previous->next = newElement;
            break;
        }
        previous = countryHead;
        countryHead = countryHead->next;
    }
    return 0;
}

int addCity(cityPosition city, char* dat)
{
    FILE* file = NULL;
    char name[20];
    int residents = 0;

    file = fopen(dat, "r");
    if (file == NULL)
    {
        perror("Couldn't open file\n");
        return -1;
    }

    while (!feof(file))
    {
        fscanf(file, "%s %d", name, &residents);
        city = insertCity(city, name, residents);
    }
    fclose(file);
    return 0;
}

cityPosition insertCity(cityPosition cityHead, char* name, int residents)
{
    cityPosition newElement = NULL;
    if (cityHead == NULL)
    {
        newElement = (cityPosition)malloc(sizeof(city));
        if (newElement == NULL)
        {
            perror("couldn't allocate memory\n");
            return NULL;
        }
        strcpy(newElement->name, name);
        newElement->residents = residents;
        newElement->left = NULL;
        newElement->right = NULL;
        return newElement;
    }
    if (residents > cityHead->residents)
        cityHead->right = insertCity(cityHead->right, name, residents);
    if (residents < cityHead->residents)
        cityHead->left = insertCity(cityHead->left, name, residents);
    if (residents == cityHead->residents)
    {
        if (strcmp(name, cityHead->name) >= 0)
            cityHead->right = insertCity(cityHead->right, name, residents);
        else
            cityHead->left = insertCity(cityHead->left, name, residents);
    }

    return cityHead;
}

int countryPrint(countryPosition countryHead)
{
    while (countryHead != NULL)
    {
        printf("\t%s\n", countryHead->name);
        cityPrint(countryHead->city->right);
        countryHead = countryHead->next;
    }
    return 0;
}

int cityPrint(cityPosition city)
{
    if (city == NULL)
        return 0;
    cityPrint(city->left);
    printf("\t\t%s\n", city->name);
    cityPrint(city->right);
    return 0;
}

countryPosition findCountry(countryPosition countryHead, char* name)
{
    while (countryHead != NULL)
    {
        if (strcmp(countryHead->name, name) == 0)
            return countryHead;
        countryHead = countryHead->next;
    }
    return countryHead;
}

int findCity(cityPosition city, int number)
{
    if (city == NULL)
        return 0;
    findCity(city->left, number);
    if (city->residents >= number)
        printf("\t%s %d\n", city->name, city->residents);
    findCity(city->right, number);
    return 0;
}

int keyForCountry(char* nameCountry)
{
    int key = 0;

    for (int i = 0; i < 5; i++)
        key += (int)nameCountry[i];

    return key % N;
}