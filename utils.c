#include "main.h"

char* utils_getCurrentTime() {
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

char* utils_getCurrentDate()
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
void utils_generateId(char placeholder[])
{
    // char chars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char nums[] = "1234567890";

    placeholder[0] = '\0';

    // get 4 random characters
    // for (int i = 0; i < 4; i++) {
    //     char temp[2] = {chars[rand() % strlen(chars)], '\0'};
    //     strcat(placeholder, temp);
    // }

    // get 8 random numbers
    for (int i = 0; i < 8; i++) {
        char temp[2] = {nums[rand() % strlen(nums)], '\0'};
        strcat(placeholder, temp);
    }
}

int utils_getCurrentMonthInt()
{
    time_t rawtime;
    struct tm *info;
    time(&rawtime);
    info = localtime(&rawtime);

    return info->tm_mon;
}

int utils_getCurrentDayInt()
{
    time_t rawtime;
    struct tm *info;
    time(&rawtime);
    info = localtime(&rawtime);

    return info->tm_mday - 1;
}

// gives the size of a linked list
int utils_getListSize(struct Node **head)
{
    struct Node *current = *head;
    int size = 0;
    // traverse the list and increment size on each iteration, then return it
    while (current != NULL) {
        current = current->next;
        size++;
    }
    return size;
}

// frees the memory of a nodes in a linked list
void utils_freeLinkedList(struct Node **head)
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

void utils_clearNewline(char *string)
{
    size_t len = strlen(string);                                                
    if (len > 0 && string[len - 1] == '\n') string[len - 1] = '\0';
}

void utils_updateDate(char *placeholder)
{
    sprintf(placeholder, "%s / %s", utils_getCurrentDate(), utils_getCurrentTime());
}

// the stocks column in inventory page (10/10)
void utils_joinStocks(char *output, int stock, int base)
{
    char stockStr[5];
    char baseStr[5];

    sprintf(stockStr, "%d", stock);
    sprintf(baseStr, "%d", base);

    strcpy(output, stockStr);
    strcat(output, "/");
    strcat(output, baseStr);
}

// turns a string into all lowercase
void utils_toLowercase(char *str) 
{
    while (*str) {
        *str = tolower((unsigned char)*str);
        str++;
    }
}

// capitalizes the first letter of a string
void utils_capitalizeFirst(char *str) 
{
    if (str[0] != '\0') { // Check if string is not empty
        str[0] = toupper((unsigned char)str[0]);
    }
}

// gets the percentage of stocks
void utils_getStringPercentage(char* output, int number, int base) 
{
    double percentage = utils_getPercentage(number, base);
    sprintf(output, "%.2f%%", percentage);
}

double utils_getPercentage(int number, int base)
{
    double percentage = ((double)number / base) * 100;
    return percentage;
}

void utils_centerText(int length, char *text) 
{
    int inputLength = strlen(text);
    char copy[inputLength + 1];
    strcpy(copy, text);

    // Calculate padding
    int padding = (length - inputLength) / 2;

    // Construct the centered text
    sprintf(text, "%*s%s%*s", padding, "", copy, padding, "");
}

void utils_clearAllNewline(char *str)
{
    char *newline;
    while ((newline = strchr(str, '\n')) != NULL) {
        *newline = '\0'; // Replace newline with null terminator
    }
}

// Removes leading and trailing whitespace from a string
void strtrim(char *str) 
{
    int start, end, len;

    // Find the first non-whitespace character from the beginning
    for (start = 0; isspace((unsigned char)str[start]); ++start);

    // Find the last non-whitespace character from the end
    len = strlen(str);
    for (end = len - 1; end >= 0 && isspace((unsigned char)str[end]); --end);

    // Shift the non-whitespace characters to the beginning of the string
    memmove(str, str + start, end - start + 1);
    
    // Null-terminate the string
    str[end - start + 1] = '\0';
}
