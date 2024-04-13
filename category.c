#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

#include "main.h"

void addCategoryHandler(char categories[][CATEGORY_NAME_LEN], int *categoriesLen)
{
    char header[] = "Adding a Category";
    char userInput[CATEGORY_NAME_LEN];

    newUserMessagePage(header, "Enter 'b' to go back", "Enter the name of new the Category:", "", "", "", "");
    fflush(stdin);
    fgets(userInput, CATEGORY_NAME_LEN, stdin);
    clearNewline(userInput);

    if (strcmp(userInput, "b") == 0 || strcmp(userInput, "B") == 0) return;

    // if the name is too short or none at all, don't proceed
    if (strlen(userInput) <= 1) {
        newUserMessagePage(header, "", "Empty names are not allowed.", "", "", "", "");
        sleep(SLEEP_TIME);
        return;
    }

    // if the name is too long, don't proceed
    if (strlen(userInput) > CATEGORY_NAME_LEN - 1) {
        newUserMessagePage(header, "", "Category name is too long, please try again.", "", "", "", "");
        sleep(SLEEP_TIME);
        return;
    }

    // if it passes all the error checks, we proceed to add it
    toLowercase(userInput);
    strcpy(categories[*categoriesLen], userInput);
    *categoriesLen += 1;

    // TODO: add to file storage

    newUserMessagePage(header, "", "Category added successfully!", "", "", "", "");
    sleep(SLEEP_TIME);
}

void deleteCategoryHandler(char categories[][CATEGORY_NAME_LEN], int *categoriesLen)
{
    char header[] = "Adding a Category";
    char userInput[2];

    if (*categoriesLen == 0) {
        newUserMessagePage(header, "Enter any key to go back", "No category to be deleted", "", "", "", "");
    } else {
        newUserMessagePage(header, "Enter 'b' to go back", "Enter the index of the item you want to delete:", "", "", "", "");
    }

    fflush(stdin);
    scanf("%s", userInput);

    if (strcmp(userInput, "b") == 0 || strcmp(userInput, "B") == 0 || *categoriesLen == 0) return;

    // convert user input(string) to int
    int index = atoi(userInput) - 1;

    if (index < 0 || index >= *categoriesLen) {
        newUserMessagePage(header, "", "Invalid index.", "", "", "", "");
        sleep(SLEEP_TIME);
        return;
    }

    // shift the items to the left, overwritting the deleted item
    for (int i = index; i < *categoriesLen - 1; i++)
        strcpy(categories[i], categories[i + 1]);

    // decrease the counter
    *categoriesLen -= 1;
    
    // TODO: delete to file storage

    newUserMessagePage(header, "", "Category deleted successfully!", "", "", "", "");
    sleep(SLEEP_TIME);
}