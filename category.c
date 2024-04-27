#include "main.h"

// handles the addition of category, like user inputs, user input validations, etc.
void category_addCategoryHandler(char categories[][CATEGORY_NAME_LEN], int *categoriesLen)
{
    char header[] = "Adding a Category";
    char userInput[CATEGORY_NAME_LEN];

    display_newUserMessagePage(header, "Enter 'b' to go back", "Enter the name of the new Category:", "", "", "", "");
    fflush(stdin);
    fgets(userInput, CATEGORY_NAME_LEN, stdin);
    utils_clearNewline(userInput);

    if (strcmp(userInput, "b") == 0 || strcmp(userInput, "B") == 0) return;

    // if the name is too short or none at all, don't proceed
    if (strlen(userInput) <= 1) {
        display_newUserMessagePage(header, "", "Empty names are not allowed.", "", "", "", "");
        sleep(SLEEP_TIME);
        return;
    }

    // if the name is too long, don't proceed
    if (strlen(userInput) > CATEGORY_NAME_LEN - 1) {
        display_newUserMessagePage(header, "", "Category name is too long, please try again.", "", "", "", "");
        sleep(SLEEP_TIME);
        return;
    }

    // if it passes all the error checks, we proceed to add it
    category_addCategory(categories, categoriesLen, userInput);
    storage_addCategoryToStorage(userInput);

    display_newUserMessagePage(header, "", "Category added successfully!", "", "", "", "");
    sleep(SLEEP_TIME);
}

void category_deleteCategoryHandler(char categories[][CATEGORY_NAME_LEN], int *categoriesLen)
{
    char header[] = "Deleting a Category";
    char userInput[2];

    if (*categoriesLen == 0) {
        display_newUserMessagePage(header, "Enter any key to go back", "No category to be deleted", "", "", "", "");
    } else {
        display_categoryPromptPage(categories, categoriesLen, "Enter the index of the category you want to delete:", "Enter 'b' to go back");
    }

    fflush(stdin);
    scanf("%s", userInput);

    if (strcmp(userInput, "b") == 0 || strcmp(userInput, "B") == 0 || *categoriesLen == 0) return;

    // convert user input(string) to int
    int index = atoi(userInput) - 1;

    if (index < 0 || index >= *categoriesLen) {
        display_newUserMessagePage(header, "", "Invalid index.", "", "", "", "");
        sleep(SLEEP_TIME);
        return;
    }

    // delete it from file storage first
    storage_deleteCategoryFromStorage(categories[index]);

    // shift the items to the left, overwritting the deleted item
    for (int i = index; i < *categoriesLen - 1; i++)
        strcpy(categories[i], categories[i + 1]);

    // decrease the counter
    *categoriesLen -= 1;
    
    display_newUserMessagePage(header, "", "Category deleted successfully!", "", "", "", "");
    sleep(SLEEP_TIME);
}

void category_editCategoryHandler(struct Node **head, char categories[][CATEGORY_NAME_LEN], int *categoriesLen)
{
    char header[] = "Editing a Category";
    char userInput[3];

    if (*categoriesLen == 0) {
        display_newUserMessagePage(header, "Enter any key to go back", "No category to be edited", "", "", "", "");
    } else {
        display_categoryPromptPage(categories, categoriesLen, "Enter the index of the category you want to edit:", "");
    }

    fflush(stdin);
    scanf("%s", userInput);
    // utils_clearAllNewline(userInput);

    if (strcmp(userInput, "b") == 0 || strcmp(userInput, "B") == 0 || *categoriesLen == 0) return;

    // convert user input(string) to int
    int index = atoi(userInput) - 1;

    if (index < 0 || index >= *categoriesLen) {
        display_newUserMessagePage(header, "", "Invalid index.", "", "", "", "");
        sleep(SLEEP_TIME);
        return;
    }

    char oldVer[CATEGORY_NAME_LEN];
    strcpy(oldVer, categories[index]);

    char message[50] = "Enter a new name for ";
    strcat(message, categories[index]);

    display_newUserMessagePage(header, "", message, "", "", "", "");
    fflush(stdin);
    fgets(categories[index], CATEGORY_NAME_LEN, stdin);
    utils_clearNewline(categories[index]);

    // update the list with the old category
    item_updateItemsCategory(head, oldVer, categories[index]);

    // update the data from storage
    storage_editCategoryFromStorage(oldVer, categories[index]);
}

// function logic to add a category to the array, also used in getting categories from storage
void category_addCategory(char categories[][CATEGORY_NAME_LEN], int *categoriesLen, char *categoryToAdd)
{
    // utils_toLowercase(categoryToAdd);
    strcpy(categories[*categoriesLen], categoryToAdd);
    *categoriesLen += 1;
}

// checks if the inputted category exists, returns 1 if true, 0 if false
int category_isValidCategory(char categories[][CATEGORY_NAME_LEN], int *categoriesLen, char *category)
{
    for (int i = 0; i < *categoriesLen; i++) 
        if (strcmp(categories[i], category) == 0) return 1;
    
    return 0;
}

// the page that pops up when asking a user for a category 
void category_itemCategoryPrompter(char *placeholder, char categories[][CATEGORY_NAME_LEN], int *categoriesLen)
{
    while (true) {
        char index[2];
        system("cls");
        display_categoryPreview(categories, categoriesLen);
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

void category_viewCategoryItems(struct Node **head, char categories[][CATEGORY_NAME_LEN], int *categoriesLen)
{
    char header[] = "Viewing items of a category";
    char userInput[2];

    if (*categoriesLen == 0) {
        display_newUserMessagePage(header, "Enter any key to go back", "No category to be viewed", "", "", "", "");
    } else {
        display_categoryPromptPage(categories, categoriesLen, "Enter the index of the category you want to view:", "Enter 'b' to go back");
    }

    fflush(stdin);
    scanf("%s", userInput);

    if (strcmp(userInput, "b") == 0 || strcmp(userInput, "B") == 0 || *categoriesLen == 0) return;

    // convert user input(string) to int
    int index = atoi(userInput) - 1;

    if (index < 0 || index >= *categoriesLen) {
        display_newUserMessagePage(header, "", "Invalid index.", "", "", "", "");
        sleep(SLEEP_TIME);
        return;
    }

    // get the items based on category input
    struct Node *categoryItems = NULL;
    item_getItemsByCategory(head, categories[index], &categoryItems);

    // render the result
    system("cls");
    display_itemCategoryPage(&categoryItems);

    // free the memory
    utils_freeLinkedList(&categoryItems);

    bannerUserInput();
    fflush(stdin);
    fgets(userInput, 2, stdin);
}
