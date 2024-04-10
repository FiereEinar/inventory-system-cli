#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

#include "main.h"

char* getCurrentTime() {
    time_t now;
    struct tm *localTime;
    // the string to store the final output
    static char timeString[12];

    // get the time using the <time.h> library and storing it in now variable
    time(&now);
    // get the local time
    localTime = localtime(&now);

    // extract the datas from the struct tm
    int hour = localTime->tm_hour;
    int minute = localTime->tm_min;
    char period[3]; // AM or PM

    // converting it into AM/PM instead of military time
    if (hour >= 12) {
        strcpy(period, "PM");
        if (hour > 12) {
            hour -= 12;
        }
    } else {
        strcpy(period, "AM");
        if (hour == 0) {
            hour = 12;
        }
    }

    // format the time string
    sprintf(timeString, "%02d:%02d %s", hour, minute, period);

    return timeString;
}

char* getCurrentDate()
{
    time_t currentTime;
    struct tm *localTime;
    static char dateString[11];

    time(&currentTime);
    localTime = localtime(&currentTime);

    // extract the datas from the struct tm
    int year = localTime->tm_year + 1900;
    int month = localTime->tm_mon + 1;
    int day = localTime->tm_mday;

    // format the date string
    sprintf(dateString, "%d-%02d-%02d", year, month, day);

    return dateString;
}

// generates an id of 4 random character and 4 random numbers
void generateId(char placeholder[])
{
    char chars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char nums[] = "1234567890";

    placeholder[0] = '\0';

    // get 4 random characters
    for (int i = 0; i < 4; i++)
    {
        char temp[2] = {chars[rand() % strlen(chars)], '\0'};
        strcat(placeholder, temp);
    }

    // get 4 random numbers
    for (int i = 0; i < 4; i++)
    {
        char temp[2] = {nums[rand() % strlen(nums)], '\0'};
        strcat(placeholder, temp);
    }
}

int getCurrentDateInt()
{
    time_t rawtime;
    struct tm *info;
    time(&rawtime);
    info = localtime(&rawtime);

    return info->tm_mon;
}

int getCurrentDayInt()
{
    time_t rawtime;
    struct tm *info;
    time(&rawtime);
    info = localtime(&rawtime);

    return info->tm_mday - 1;
}

// gives the size of a linked list
int getListSize(struct Node **head)
{
    struct Node *current = *head;
    int size = 0;
    // traverse the list and increment size on each iteration, then return it
    while (current != NULL)
    {
        current = current->next;
        size++;
    }
    return size;
}

// frees the memory of a nodes in a linked list
void freeLinkedList(struct Node **head)
{
    struct Node *current = *head;
    struct Node *next;

    while (current != NULL) {
        // store the next node before freeing the current one
        next = current->next;
        // free the memory allocated for the current node
        free(current);
        // move to the next node
        current = next;
    }
}

void clearNewline(char *string)
{
    size_t len = strlen(string);                                                
    if (len > 0 && string[len - 1] == '\n') string[len - 1] = '\0';
}

void updateDate(char *placeholder)
{
    sprintf(placeholder, "%s / %s", getCurrentDate(), getCurrentTime());
}

// the stocks column in inventory page (10/10)
void joinStocks(char *output, int stock, int base)
{
    char stockStr[5];
    char baseStr[5];

    sprintf(stockStr, "%d", stock);
    sprintf(baseStr, "%d", base);

    strcpy(output, stockStr);
    strcat(output, "/");
    strcat(output, baseStr);
}