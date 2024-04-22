#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <conio.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

#include "main.h"

// handles the process of restocking an item
void restockItemHandler(struct Node **head, struct ReportPerMonth monthlyProfits[])
{
    if (*head == NULL){
        newUserMessagePage("Restocking an Item", "Enter any key to go back", "No item to restock.", "", "", "", "");
    } else {
        inventoryPromptPage(head, "Enter the ID of the item you want to restock:", "Enter 'b' to go back");
    }
    
    int addedStocks;
    double additionalCosts;

    // ask for id
    char itemId[ID_LENGTH];
    scanf("%s", itemId);

    if (strcmp(itemId, "b") == 0 || strcmp(itemId, "B") == 0 || *head == NULL) return;

    // get the item based on id
    struct Node *current = getItemById(head, itemId);

    // if we didn't find the item, give them an error
    if (current == NULL) {
        newUserMessagePage("Restocking an Item", "", "Error: Item does not exist. Please try again", "", "", "", "");
        sleep(SLEEP_TIME);
        return;
    }

    char messageWithName[100];
    strcpy(messageWithName, "Enter the amount of stocks added in ");
    strcat(messageWithName, current->data.name);
    newUserMessagePage("Restocking an Item", "", messageWithName, "", "", "", "");
    scanf("%d", &addedStocks);

    // update the stocks and base stocks
    current->data.stocks += addedStocks;
    if (current->data.stocks > current->data.baseStocks) current->data.baseStocks = current->data.stocks;

    newUserMessagePage("Restocking an Item", "", "Enter additional costs if there are any:", "", "", "", "");
    scanf("%lf", &additionalCosts);

    // tally the costs
    updateCosts(monthlyProfits, addedStocks, current->data.originalPrice);
    updateAdditionalCosts(monthlyProfits, additionalCosts);
    
    // update the data from storage
    updateReportsFromStorage(monthlyProfits);
    editItemFromStorageById(current->data.id, current->data);

    updateDate(current->data.lastUpdated);
    newUserMessagePage("Restocking an Item", "", "Item restocked successfully!", "", "", "", "");
    sleep(SLEEP_TIME);
}

// gives you the item based on ID, returns NULL if not found
struct Node *getItemById(struct Node **list, char itemId[])
{
    struct Node *current = *list;

    while (current != NULL) {
        if (strcmp(current->data.id, itemId) == 0) return current;
        
        current = current->next;
    }
    return NULL;
}

// handles the searching of an item
void searchItemHandler(struct Node **list)
{
    if (*list == NULL) {
        newUserMessagePage("Searching an Item", "Enter any key to go back", "No item to search.", "", "", "", "");
    } else {
        newUserMessagePage("Searching an Item", "Enter 'b' to go back.", "Enter a keyword to search: ", "", "", "", "");
    }

    char searchTerm[NAME_SIZE];
    // create a current variable to prevent the head pointer from being manipulated
    struct Node *current = *list;
    // another linked list to store the results
    struct Node *results = NULL;

    fflush(stdin);
    fgets(searchTerm, NAME_SIZE, stdin);

    clearNewline(searchTerm);

    if (strcmp(searchTerm, "b") == 0 || strcmp(searchTerm, "B") == 0 || *list == NULL) return;

    // we traverse the list and on each iteration, check if searchTerm is a substring of the current data name
    while(current != NULL) {
        // strstr() is a function in <string.h> that will check if a string is a substring of another string. in simple term, if it fits the searchTerm
        if(strstr(current->data.name, searchTerm) != NULL) 
            addItemToLinkedList(&results, current->data);
        // go to next node (iterator)
        current = current->next;
    }

    // render the results
    system("cls");
    inventoryPromptPage(&results, "", "Enter any key to go back.");
    // free the memory of results
    freeLinkedList(&results);

    // to be able to go back to inventory page
    getch();
}

// handles the process of prompting the user for an item to be added
void addItemHandler(struct Node **head, struct ReportPerMonth monthlyProfits[], char categories[][CATEGORY_NAME_LEN], int *categoriesLen)
{
    // using item struct to store values, less variable declaration needed
    struct Item newItem;
    double additionalCosts;
    char message[][100] = 
    {
        "Enter the name of new item: ",
        "Enter the the initial stocks of item: ",
        "Enter the selling price of item: ",
        "Enter the original price of item: ",
        "Enter additional costs if there are any:",
        "Enter the index of category for the new item. Press enter to skip"
    };

    newUserMessagePage("Adding an Item", "Enter 'b' to go back", message[0], "", "", "", "");
    fflush(stdin);
    fgets(newItem.name, NAME_SIZE, stdin);
    clearNewline(newItem.name);

    if (strcmp(newItem.name, "b") == 0 || strcmp(newItem.name, "B") == 0) return;

    // if the name is too short or none at all, don't proceed
    if (strlen(newItem.name) <= 1) {
        newUserMessagePage("Adding an Item", "Enter 'b' to go back", "Items with no name are not allowed.", "", "", "", "");
        sleep(SLEEP_TIME);
        return;
    }

    // if the name is too long, don't proceed
    if (strlen(newItem.name) > NAME_SIZE - 1) {
        newUserMessagePage("Adding an Item", "Enter 'b' to go back", "Item name is too long, please try again.", "", "", "", "");
        sleep(SLEEP_TIME);
        return;
    }

    // ask the user for the category
    itemCategoryPrompter(newItem.category, categories, categoriesLen);

    newUserMessagePage("Adding an Item", "", message[1], "", "", "", "");
    scanf("%d", &newItem.stocks);
    newItem.baseStocks = newItem.stocks;

    newUserMessagePage("Adding an Item", "", message[2], "", "", "", "");
    scanf("%lf", &newItem.price);

    newUserMessagePage("Adding an Item", "", message[3], "", "", "", "");
    scanf("%lf", &newItem.originalPrice);

    newUserMessagePage("Adding an Item", "", message[4], "", "", "", "");
    scanf("%lf", &additionalCosts);
    
    // after getting all the necessary data from user, add it to the list
    addItemToList(head, monthlyProfits, newItem.name, newItem.stocks, newItem.price, newItem.originalPrice, newItem.category, additionalCosts);
    newUserMessagePage("Adding an Item", "", "Item added succesfully!", "", "", "", "");
    sleep(SLEEP_TIME);
}

// handles the process of deleting an item on the inventory based on ID
void deleteItemHandler(struct Node **head, struct ReportPerMonth monthlyProfits[])
{
    if (*head == NULL) {
        newUserMessagePage("Deleting an Item", "Enter any key to go back", "No item to delete.", "", "", "", "");
    } else {
        inventoryPromptPage(head, "Enter the ID of the item you want to delete:", "Enter 'b' to go back");
    }

    // get the ID to be deleted
    char idToDelete[ID_LENGTH];
    scanf("%s", idToDelete);

    if (strcmp(idToDelete, "b") == 0 || strcmp(idToDelete, "B") == 0 || *head == NULL) return;

    // we create a current variable so that we don't manipulate the pointer of the head
    struct Node *current = *head;
    // we keep track of the deleted Node to free its memory since it was dynamically allocated
    struct Node *deleted = NULL;

    // if the user is trying to delete the first item, we simply set the head to point to the next node
    if (strcmp(current->data.id, idToDelete) == 0) {
        deleted = *head;

        // ask the user if he/she wants to deduct the total costs of the deleted item
        double deduction = deleted->data.originalPrice * (double)deleted->data.stocks;
        reflectToMonthlyCostsOnDeletion(monthlyProfits, deduction);

        if (current->next == NULL) *head = NULL;
        else *head = current->next;
    } else {
        // to delete a Node, we traverse to the Node right before it
        while(current->next != NULL) {
            // if the next item is the item to be deleted, then break
            if (strcmp(current->next->data.id, idToDelete) == 0) break;
            current = current->next;
        }
        // if we didn't find the item, give them an error
        if (current->next == NULL || strcmp(current->next->data.id, idToDelete) != 0) {
            newUserMessagePage("Deleting an Item", "", "Error: Item does not exist. Please try again.", "", "", "", "");
            sleep(SLEEP_TIME);
            return;
        }

        deleted = current->next;

        // ask the user if he/she wants to deduct the total costs of the deleted item
        double deduction = deleted->data.originalPrice * (double)deleted->data.stocks;
        reflectToMonthlyCostsOnDeletion(monthlyProfits, deduction);

        // delete it by overwritting it
        if (current->next->next == NULL) current->next = NULL;
        else current->next = current->next->next;
    }

    // delete it from the storage as well
    deleteItemFromStorageById(idToDelete);
    newUserMessagePage("Deleting an Item", "", "Item deleted succesfully!", "", "", "", "");
    // free the memory
    free(deleted);
    sleep(SLEEP_TIME);
}

// asks the user if he/she wants to deduct the total costs of the deleted item
void reflectToMonthlyCostsOnDeletion(struct ReportPerMonth monthlyProfits[], double deduction)
{
    char action;

    newUserMessagePage("Deleting an Item", "", "Do you want to deduct the total costs of deleted item to your monthly costs report?[y/n]", "", "", "", "");
    fflush(stdin);
    scanf("%c", &action);

    // TODO: account for additional costs
    if (action == 'y' || action == 'Y') {
        reduceCosts(monthlyProfits, deduction);
        updateReportsFromStorage(monthlyProfits);
    }
}

// handles the editing of an item
void editItemHandler(struct Node **head, char categories[][CATEGORY_NAME_LEN], int *categoriesLen)
{
    if (*head == NULL) {
        newUserMessagePage("Editing an Item", "Enter any key to go back", "No item to edit.", "", "", "", "");
    } else {
        inventoryPromptPage(head, "Enter the ID of the item you want to edit:", "Enter 'b' to go back");
    }

    // the options to edit
    int toUpdate;
    // this will hold the index of item to be deleted
    char itemId[ID_LENGTH];
    scanf("%s", itemId);

    if (strcmp(itemId, "b") == 0 || strcmp(itemId, "B") == 0 || *head == NULL) return;

    // get the item
    struct Node *current = getItemById(head, itemId);

    // if we didn't find the item, give them an error
    if (current == NULL) {
        newUserMessagePage("Editing an Item", "", "Error: Item does not exist. Please try again", "", "", "", "");
        sleep(SLEEP_TIME);
        return;
    }

    newUserMessagePage("Editing an Item", "", "Options:", "[ 1. name | 2. category | 3. selling price | 4. original price | 5. base stocks ]", "What do you want to edit?: ", "", "");
    scanf("%d", &toUpdate);

    // this is just to dynamically show a message with the name of item
    char headerWithName[100];
    // after that, we simply determine what the user wants to update and prompt the user for new data
    switch(toUpdate) {
        // EDITED: NAME
        case 1:
            strcpy(headerWithName, "Enter a new name for ");
            strcat(headerWithName, current->data.name);
            newUserMessagePage("Editing an Item", "", headerWithName, "", "", "", "");

            fflush(stdin);
            fgets(current->data.name, NAME_SIZE, stdin);
            clearNewline(current->data.name);
            break;
        // EDITED: category
        case 2:
            itemCategoryPrompter(current->data.category, categories, categoriesLen);
            break;
        // EDITED: selling price
        case 3:
            strcpy(headerWithName, "Enter an updated selling price of ");
            strcat(headerWithName, current->data.name);
            newUserMessagePage("Editing an Item", "", headerWithName, "", "", "", "");

            scanf("%lf", &current->data.price);
            current->data.profit = current->data.price - current->data.originalPrice;
            break;
        // EDITED: original price
        case 4:
            strcpy(headerWithName, "Enter an updated original price of ");
            strcat(headerWithName, current->data.name);
            newUserMessagePage("Editing an Item", "", headerWithName, "", "", "", "");
            
            scanf("%lf", &current->data.originalPrice);
            current->data.profit = current->data.price - current->data.originalPrice;
            break;
        // EDITED: base stocks
        case 5:
            strcpy(headerWithName, "Enter an updated base stocks of ");
            strcat(headerWithName, current->data.name);
            newUserMessagePage("Editing an Item", "", headerWithName, "", "", "", "");

            scanf("%d", &current->data.baseStocks);
            break;
        default:
            newUserMessagePage("Editing an Item", "", "Incorrect number entered", "", "", "", "");
            return;
    }

    updateDate(current->data.lastUpdated);
    // update the item in the storage
    editItemFromStorageById(itemId, current->data);

    newUserMessagePage("Editing an Item", "", "Item edited succesfully!", "", "", "", "");
    sleep(SLEEP_TIME);
}

// handles the process of setting the data and tallying the costs, records of an item being added
void addItemToList(struct Node **head, struct ReportPerMonth monthlyProfits[], char name[], int stocks, double price, double originalPrice, char category[], double additionalCost)
{
    // tbh, i could've just passed the entire Item struct, but im using this function for generating test items as well so...
    struct Item newItem;

    // append all the data
    strcpy(newItem.name, name);
    strcpy(newItem.category, category);
    newItem.stocks = stocks;
    newItem.baseStocks = stocks;
    newItem.price = price;
    newItem.originalPrice = originalPrice;
    newItem.profit = newItem.price - newItem.originalPrice;

    updateDate(newItem.dateAdded);
    updateDate(newItem.lastUpdated);

    generateId(newItem.id);

    // tally the costs
    updateCosts(monthlyProfits, newItem.stocks, newItem.originalPrice);
    updateAdditionalCosts(monthlyProfits, additionalCost);
    // update the storage
    updateReportsFromStorage(monthlyProfits);

    addItemToLinkedList(head, newItem);
    addItemToStorage(newItem);
}

// handles the actual addition to the list
// REMINDER: use the function freeLinkedList() after using this funciton because it dynamically allocates memory
void addItemToLinkedList(struct Node **head, struct Item item)
{
    struct Item newItem = item;                 // TODO: remove this
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    newNode->data = newItem;
    newNode->next = NULL;

    // the logic for adding it to the list
    // if this is the first item, add it in the head
    if (*head == NULL) {
        *head = newNode;
    } else {
        // if not, go to the end and add it there
        struct Node *current = *head;
        while(current->next != NULL) current = current->next;
        
        current->next = newNode;
    }
}

// handles the process of viewing more details of an item by asking for ID
void viewItemDetails(struct Node **head)
{
    char itemId[ID_LENGTH];
    if (*head == NULL){
        newUserMessagePage("Viewing an Item", "Enter any key to go back", "No item to view.", "", "", "", "");
    } else {
        inventoryPromptPage(head, "Enter the ID of the item you want to view:", "Enter 'b' to go back");
    }
    scanf("%s", itemId);

    if (strcmp(itemId, "b") == 0 || strcmp(itemId, "B") == 0 || *head == NULL) {
        system("cls");
        inventoryPage(head);
        return;
    }

    // get the item
    struct Node *itemData = getItemById(head, itemId);

    if (itemData == NULL) {
        newUserMessagePage("Viewing an Item", "", "Item not found, please try again.", "", "", "", "");
        sleep(SLEEP_TIME);
        return;
    }

    // render it if it is found
    system("cls");
    itemDataPage(itemData->data);

    // to be able to go back to inventory page
    char x;
    bannerUserInput();
    fflush(stdin);
    scanf("%c", &x);
}

// updates the category of the affected items in both the list and file storage. could be optimized but im not willing to write any more code
void updateItemsCategory(struct Node **head, char *oldCategory, char *newCategory)
{
    struct Node *current = *head;

    while (current != NULL) {
        // if it matches the old category, replace it with the new one
        if (strcmp(current->data.category, oldCategory) == 0) {
            strcpy(current->data.category, newCategory);
            editItemFromStorageById(current->data.id, current->data);
        }
        current = current->next;
    }
}

// gets the items with the given category and appends it in your desired placeholder
void getItemsByCategory(struct Node **head, char *category, struct Node **placeholder)
{
    struct Node *current = *head;

    while (current != NULL) {
        // if the current item has the matching category, add it to the placeholder
        if (strcmp(current->data.category, category) == 0) {
            addItemToLinkedList(placeholder, current->data);
        }
        current = current->next;
    }
}

void getStockStatus(char *status, int stocks, int baseStocks)
{
    double percentage = ((double)stocks / baseStocks) * 100.0;

    if (percentage >= 30.0) {
        strcpy(status, "In Stock");
        return;
    } else if (percentage >= 0.1) {
        strcpy(status, "Low Stock");
        return;
    } else {
        strcpy(status, "No Stock");
        return;
    }
}
