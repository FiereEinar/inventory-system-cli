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
