#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

#include "main.h"

void sellItemHandler(struct Node **head, struct ReportPerMonth monthlyProfits[])
{
    newUserMessagePage("Point of Sale", "Enter 'b' to go back", "Enter the item ID: ", "", "", "", "");

    char itemId[ID_LENGTH];
    double quantity;

    scanf("%s", itemId);

    struct Node *current = getItemById(head, itemId);

    if (strcmp(itemId, "b") == 0 || strcmp(itemId, "B") == 0) return;

    if (current == NULL) {
        newUserMessagePage("Point of Sale", "", "Item not found, please try again.", "", "", "", "");
        sleep(SLEEP_TIME);
        return;
    }

    newUserMessagePage("Point of Sale", "", "Enter the quantity purchased: ", "", "", "", "");
    scanf("%lf", &quantity);

    // if there's not enough stocks, don't proceed
    if (current->data.stocks < quantity)
    {
        newUserMessagePage("Point of Sale", "", "Not enough stocks to fulfill the purchase.", "", "", "", "");
        sleep(SLEEP_TIME);
        return;
    }

    // once the item is found, update the stocks and tally the revenue
    current->data.stocks -= (int)quantity;
    monthlyProfits[getCurrentDateInt()].revenue += quantity * current->data.price;
    monthlyProfits[getCurrentDateInt()].day[getCurrentDayInt()].revenue += quantity * current->data.price;

    monthlyProfits[getCurrentDateInt()].profit += quantity * current->data.profit;
    monthlyProfits[getCurrentDateInt()].day[getCurrentDayInt()].profit += quantity * current->data.profit;

    editItemFromStorageById(current->data.id, current->data);

    newUserMessagePage("Point of Sale", "", "Item purchased successfully", "", "", "", "");
    sleep(SLEEP_TIME);
}

void restockItem(struct Node **head, struct ReportPerMonth monthlyProfits[])
{
    if (*head == NULL){
        newUserMessagePage("Restocking an Item", "Enter any key to go back", "No item to restock.", "", "", "", "");
    } else {
        newUserMessagePage("Restocking an Item", "Enter 'b' to go back", "Enter the ID of the item: ", "", "", "", "");
    }
    
    int addedStocks;
    double additionalCosts;
    char idToDelete[ID_LENGTH];

    // prompt the user for some details
    scanf("%s", idToDelete);

    if (strcmp(idToDelete, "b") == 0 || strcmp(idToDelete, "B") == 0 || *head == NULL) return;
    // we create a current variable so that we don't manipulate the pointer of the head
    struct Node *current = getItemById(head, idToDelete);
    // traverse to the Node/item that the user wants to update

    // if we didn't find the item, give them an error
    if (current == NULL)
    {
        newUserMessagePage("Restocking an Item", "", "Error: Item does not exist. Please try again", "", "", "", "");
        sleep(SLEEP_TIME);
        return;
    }

    char messageWithName[100];
    strcpy(messageWithName, "Enter the amount of stocks added in ");
    strcat(messageWithName, current->data.name);
    newUserMessagePage("Restocking an Item", "", messageWithName, "", "", "", "");

    scanf("%d", &addedStocks);
    current->data.stocks += addedStocks;
    current->data.baseStocks = current->data.stocks;

    newUserMessagePage("Restocking an Item", "", "Enter additional costs (gas, transportation, etc.)", "", "", "", "");
    scanf("%lf", &additionalCosts);

    // tally the costs
    monthlyProfits[getCurrentDateInt()].costs += (addedStocks * current->data.originalPrice) + additionalCosts;
    monthlyProfits[getCurrentDateInt()].day[getCurrentDayInt()].costs += (addedStocks * current->data.originalPrice) + additionalCosts;

    monthlyProfits[getCurrentDateInt()].additionalCosts += additionalCosts;
    monthlyProfits[getCurrentDateInt()].day[getCurrentDayInt()].additionalCosts += additionalCosts;

    updateDate(current->data.lastUpdated);
    editItemFromStorageById(current->data.id, current->data);

    newUserMessagePage("Restocking an Item", "", "Item restocked successfully!", "", "", "", "");
    sleep(SLEEP_TIME);
}

struct Node *getItemById(struct Node **list, char itemId[])
{
    struct Node *current = *list;

    while (current != NULL)
    {
        if (strcmp(current->data.id, itemId) == 0) return current;
        
        current = current->next;
    }
    return NULL;
}

void searchItem(struct Node **list)
{
    if (*list == NULL) {
        newUserMessagePage("Searching an Item", "Enter any key to go back", "No item to search.", "", "", "", "");
    } else {
        newUserMessagePage("Searching an Item", "Enter 'b' to go back.", "Enter a keyword to search: ", "", "", "", "");
    }

    char searchTerm[20];
    struct Node *current = *list;
    // another linked list to store the results
    struct Node *results = NULL;

    fflush(stdin);
    fgets(searchTerm, 20, stdin);

    if (strcmp(searchTerm, "b") == 0 || strcmp(searchTerm, "B") == 0 || *list == NULL) return;

    size_t len = strlen(searchTerm);                                                // clear the newline character from fgets()
    if (len > 0 && searchTerm[len - 1] == '\n') searchTerm[len - 1] = '\0';

    // we traverse the list and on each iteration, check if searchTerm is a substring of the current data name
    while(current != NULL) {
        // strstr() is a function in <string.h> that will check if a string is a substring of another string. in simple term, if it fits the searchTerm
        if(strstr(current->data.name, searchTerm) != NULL) 
            addItemToLinkedList(&results, current->data);
        // go to next node (iterator)
        current = current->next;
    }

    system("cls");
    inventoryPage(&results);
    // free the memory of results
    freeLinkedList(&results);
}

void addItemHandler(struct Node **head, struct ReportPerMonth monthlyProfits[])
{
    // using item struct to store values, less variable declaration needed
    struct Item newItem;
    double additionalCosts;
    char message[][60] = 
    {
        "Enter the name of new item: ",
        "Enter the the initial stocks of item: ",
        "Enter the selling price of item: ",
        "Enter the original price of item: ",
        "Enter additional costs (gas, transportation, etc.)"
    };

    newUserMessagePage("Adding an Item", "Enter 'b' to go back", message[0], "", "", "", "");
    fflush(stdin);
    fgets(newItem.name, NAME_SIZE, stdin);
    
    if (strcmp(newItem.name, "b") == 0 || strcmp(newItem.name, "B") == 0) return;

    size_t len = strlen(newItem.name);                  // clear the newline character from fgets()
    if (len > 0 && newItem.name[len - 1] == '\n') newItem.name[len - 1] = '\0';

    if (len <= 1) {
        newUserMessagePage("Adding an Item", "Enter 'b' to go back", "Items with no name are not allowed.", "", "", "", "");
        sleep(SLEEP_TIME);
        return;
    }

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
    addItemToList(head, monthlyProfits, newItem.name, newItem.stocks, newItem.price, newItem.originalPrice, additionalCosts);
    newUserMessagePage("Adding an Item", "", "Item added succesfully!", "", "", "", "");
    sleep(SLEEP_TIME);
}

void deleteItemHandler(struct Node **head, struct ReportPerMonth monthlyProfits[])
{
    if (*head == NULL) {
        newUserMessagePage("Deleting an Item", "Enter any key to go back", "No item to delete.", "", "", "", "");
    } else {
        newUserMessagePage("Deleting an Item", "Enter 'b' to go back", "Enter the ID of the item you want to delete: ", "", "", "", "");
    }
    // this will hold the index of item to be deleted
    char idToDelete[ID_LENGTH];
    scanf("%s", idToDelete);

    if (strcmp(idToDelete, "b") == 0 || strcmp(idToDelete, "B") == 0 || *head == NULL) return;

    // we create a current variable so that we don't manipulate the pointer of the head
    struct Node *current = *head;
    // we keep track of the deleted Node to free its memory since it was dynamically allocated
    struct Node *deleted = NULL;

    // if the user is trying to delete the first item, we simply set the head to point to the next node
    if (strcmp(current->data.id, idToDelete) == 0) {
        double deduction = current->data.originalPrice * current->data.stocks;
        reflectToMonthlyCostsOnDeletion(monthlyProfits, deduction);

        deleted = *head;
        if (current->next == NULL) *head = NULL;
        else *head = current->next;
    } else {
        // to delete a Node, we traverse to the Node right before the Node TO BE DELETED
        while(current->next != NULL)
        {
            // if the next item is the item to be deleted, then break
            if (strcmp(current->next->data.id, idToDelete) == 0) break;
            current = current->next;
        }
        // if we didn't find the item, give them an error
        if (current->next == NULL || strcmp(current->next->data.id, idToDelete) != 0)
        {
            newUserMessagePage("Deleting an Item", "", "Error: Item does not exist. Please try again.", "", "", "", "");
            sleep(SLEEP_TIME);
            return;
        }
        double deduction = current->data.originalPrice * current->data.stocks;
        reflectToMonthlyCostsOnDeletion(monthlyProfits, deduction);
        // if we did find it and break out the loop, proceed here
        deleted = current->next;
        // now that were at the Node right before the Node TO BE DELETED,
        // the (current->next) is the TO BE DELETED so we simply overwrite it
        // by setting it to point to the next Node of the Node TO BE DELETED
        if (current->next->next == NULL) current->next = NULL;
        else current->next = current->next->next;
    }

    deleteItemFromStorageById(idToDelete);
    newUserMessagePage("Deleting an Item", "", "Item deleted succesfully!", "", "", "", "");
    // free the memory
    free(deleted);
    sleep(SLEEP_TIME);
}

void reflectToMonthlyCostsOnDeletion(struct ReportPerMonth monthlyProfits[], double deduction)
{
    char action;

    newUserMessagePage("Deleting an Item", "", "Do you want to deduct the total costs of deleted item to your monthly costs report?[y/n]", "", "", "", "");
    fflush(stdin);
    scanf("%c", &action);

    if (action == 'y' || action == 'Y') {
        monthlyProfits[getCurrentDateInt()].costs -= deduction;
        monthlyProfits[getCurrentDateInt()].day[getCurrentDayInt()].costs -= deduction;
    }
}

void editItemHandler(struct Node **head)
{
    if (*head == NULL) {
        newUserMessagePage("Editing an Item", "Enter any key to go back", "No item to edit.", "", "", "", "");
    } else {
        newUserMessagePage("Editing an Item", "Enter 'b' to go back", "Enter the ID of the item: ", "", "", "", "");
    }

    int toUpdate;
    // this will hold the index of item to be deleted
    char idToDelete[ID_LENGTH];
    scanf("%s", idToDelete);

    if (strcmp(idToDelete, "b") == 0 || strcmp(idToDelete, "B") == 0 || *head == NULL) return;

    struct Node *current = getItemById(head, idToDelete);

    // if we didn't find the item, give them an error
    if (current == NULL)
    {
        newUserMessagePage("Editing an Item", "", "Error: Item does not exist. Please try again", "", "", "", "");
        sleep(SLEEP_TIME);
        return;
    }

    newUserMessagePage("Editing an Item", "", "Options: [ 1. name | 2. stocks | 3. selling price | 4. original price ]", "What do you want to edit?: ", "", "", "");
    scanf("%d", &toUpdate);

    char headerWithName[100];
    char x;
    // after that, we simply determine what the user wants to update and prompt the user for new data
    switch(toUpdate) {
        case 1:
            strcpy(headerWithName, "Enter a new name for ");
            strcat(headerWithName, current->data.name);
            newUserMessagePage("Editing an Item", "", headerWithName, "", "", "", "");

            fflush(stdin);
            fgets(current->data.name, NAME_SIZE, stdin);

            // clear the newline character from fgets()
            size_t len = strlen(current->data.name);
            if (len > 0 && current->data.name[len - 1] == '\n') current->data.name[len - 1] = '\0';
            break;
        case 2:
            strcpy(headerWithName, "Enter an updated stocks price of ");
            strcat(headerWithName, current->data.name);
            newUserMessagePage("Editing an Item", "", headerWithName, "", "", "", "");

            scanf("%d", &current->data.stocks);
            newUserMessagePage("Editing an Item", "", "Do you want to update the base stocks as well? [y/n]", "", "", "", "");
            scanf("%c", &x);
            if (x == 'y') current->data.baseStocks = current->data.stocks;
            break;
        case 3:
            strcpy(headerWithName, "Enter an updated selling price of ");
            strcat(headerWithName, current->data.name);
            newUserMessagePage("Editing an Item", "", headerWithName, "", "", "", "");

            scanf("%lf", &current->data.price);
            current->data.profit = current->data.price - current->data.originalPrice;
            break;
        case 4:
            strcpy(headerWithName, "Enter an updated original price of ");
            strcat(headerWithName, current->data.name);
            newUserMessagePage("Editing an Item", "", headerWithName, "", "", "", "");
            
            scanf("%lf", &current->data.originalPrice);
            current->data.profit = current->data.price - current->data.originalPrice;
            break;
        default:
            newUserMessagePage("Editing an Item", "", "Incorrect number entered", "", "", "", "");
            return;
    }

    editItemFromStorageById(idToDelete, current->data);
    updateDate(current->data.lastUpdated);

    newUserMessagePage("Editing an Item", "", "Item edited succesfully!", "", "", "", "");
    sleep(SLEEP_TIME);
}

void addItemToList(struct Node **head, struct ReportPerMonth monthlyProfits[], char name[], int stocks, double price, double originalPrice, double additionalCost)
{
    // tbh, i could've just passed the entire Item struct, but im using this function for generating test items as well so...
    struct Item newItem;

    // append all the data
    strcpy(newItem.name, name);
    newItem.stocks = stocks;
    newItem.baseStocks = stocks;
    newItem.price = price;
    newItem.originalPrice = originalPrice;
    newItem.profit = newItem.price - newItem.originalPrice;

    updateDate(newItem.dateAdded);
    updateDate(newItem.lastUpdated);

    generateId(newItem.id);
    // tally the costs
    monthlyProfits[getCurrentDateInt()].costs += (newItem.stocks * newItem.originalPrice) + additionalCost;
    monthlyProfits[getCurrentDateInt()].day[getCurrentDayInt()].costs += (newItem.stocks * newItem.originalPrice) + additionalCost;

    monthlyProfits[getCurrentDateInt()].additionalCosts += additionalCost;
    monthlyProfits[getCurrentDateInt()].day[getCurrentDayInt()].additionalCosts += additionalCost;

    addItemToLinkedList(head, newItem);
    addItemToStorage(newItem);
}

void addItemToLinkedList(struct Node **head, struct Item item)
{
    struct Item newItem = item;
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

void viewItemDetails(struct Node **head)
{
    char itemId[ID_LENGTH];

    newUserMessagePage("Viewing an Item", "Enter 'b' to go back", "Enter the item ID: ", "", "", "", "");
    scanf("%s", itemId);

    if (strcmp(itemId, "b") == 0 || strcmp(itemId, "B") == 0 || *head == NULL) {
        system("cls");
        inventoryPage(head);
        return;
    }

    struct Node *itemData = getItemById(head, itemId);

    if (itemData == NULL) {
        newUserMessagePage("Viewing an Item", "", "Item not found, please try again.", "", "", "", "");
        sleep(SLEEP_TIME);
        system("cls");
        inventoryPage(head);
        return;
    }

    system("cls");
    itemDataPage(itemData->data);
}