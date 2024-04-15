#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

#include "main.h"

// handles the addition of category, like user inputs, user input validations, etc.
void addCategoryHandler(char categories[][CATEGORY_NAME_LEN], int *categoriesLen)
{
    char header[] = "Adding a Category";
    char userInput[CATEGORY_NAME_LEN];

    newUserMessagePage(header, "Enter 'b' to go back", "Enter the name of the new Category:", "", "", "", "");
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
    addCategory(categories, categoriesLen, userInput);
    addCategoryToStorage(userInput);

    newUserMessagePage(header, "", "Category added successfully!", "", "", "", "");
    sleep(SLEEP_TIME);
}

// function logic to add a category to the array, also used in getting categories from storage
void addCategory(char categories[][CATEGORY_NAME_LEN], int *categoriesLen, char *categoryToAdd)
{
    // toLowercase(categoryToAdd);
    strcpy(categories[*categoriesLen], categoryToAdd);
    *categoriesLen += 1;
}

void deleteCategoryHandler(char categories[][CATEGORY_NAME_LEN], int *categoriesLen)
{
    char header[] = "Deleting a Category";
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

    // delete it from file storage first
    deleteCategoryFromStorage(categories[index]);

    // shift the items to the left, overwritting the deleted item
    for (int i = index; i < *categoriesLen - 1; i++)
        strcpy(categories[i], categories[i + 1]);

    // decrease the counter
    *categoriesLen -= 1;
    
    newUserMessagePage(header, "", "Category deleted successfully!", "", "", "", "");
    sleep(SLEEP_TIME);
}

void editCategoryHandler(struct Node **head, char categories[][CATEGORY_NAME_LEN], int *categoriesLen)
{
    char header[] = "Editing a Category";
    char userInput[2];

    if (*categoriesLen == 0) {
        newUserMessagePage(header, "Enter any key to go back", "No category to be deleted", "", "", "", "");
    } else {
        newUserMessagePage(header, "Enter 'b' to go back", "Enter the index of the item you want to edit:", "", "", "", "");
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

    char oldVer[CATEGORY_NAME_LEN];
    strcpy(oldVer, categories[index]);

    char message[50] = "Enter a new name for ";
    strcat(message, categories[index]);

    newUserMessagePage(header, "Enter 'b' to go back", message, "", "", "", "");
    fflush(stdin);
    fgets(categories[index], CATEGORY_NAME_LEN, stdin);
    clearNewline(categories[index]);

    // update the list with the old category
    updateItemsWithCategory(head, oldVer, categories[index]);

    // update the data from storage
    editCategoryFromStorage(oldVer, categories[index]);
}

// checks if the inputted category exists, returns 1 if true, 0 if false
int isValidCategory(char categories[][CATEGORY_NAME_LEN], int *categoriesLen, char *category)
{
    for (int i = 0; i < *categoriesLen; i++) 
        if (strcmp(categories[i], category) == 0) return 1;
    
    return 0;
}

// the page that pops up when asking a user for a category 
void itemCategoryPrompter(char *placeholder, char categories[][CATEGORY_NAME_LEN], int *categoriesLen)
{
    while (true) {
        if (categoriesLen == 0) {
            strcpy(placeholder, "none");
            break;
        }

        char index[2];
        system("cls");
        categoryPreview(categories, categoriesLen);
        bannerUserInput();
        scanf("%s", index);

        if (strcmp(index, "n") == 0) {
            strcpy(placeholder, "none");
            break;
        }

        // convert string to int 
        int n = atoi(index) - 1;

        // if the given index is valid, then break and proceed
        if (n >= 0 && n < *categoriesLen) {
            strcpy(placeholder, categories[n]);
            break;
        }
    }
}

void viewCategoryItems(struct Node **head, char categories[][CATEGORY_NAME_LEN], int *categoriesLen)
{
    char header[] = "Viewing items of a category";
    char userInput[2];

    if (*categoriesLen == 0) {
        newUserMessagePage(header, "Enter any key to go back", "No category to be viewed", "", "", "", "");
    } else {
        newUserMessagePage(header, "Enter 'b' to go back", "Enter the index of the item you want to view:", "", "", "", "");
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

    // get the items based on category input
    struct Node *categoryItems = NULL;
    getItemsByCategory(head, categories[index], &categoryItems);

    // render the result
    system("cls");
    itemCategoryPage(&categoryItems);

    // free the memory
    freeLinkedList(&categoryItems);

    bannerUserInput();
    fflush(stdin);
    fgets(userInput, 2, stdin);
}
