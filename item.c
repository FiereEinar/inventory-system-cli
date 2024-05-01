#include "main.h"

// handles the process of restocking an item
void item_restockItemHandler(struct Node **head, struct ReportPerMonth monthlyProfits[])
{
    bool listIsEmpty = *head == NULL;

    if (listIsEmpty) {
        display_newUserMessagePage("Restocking an Item", "Enter any key to go back", "No item to restock.", "", "", "", "");
    } else {
        display_inventoryPromptPage(head, "Enter the ID of the item you want to restock:", "Enter 'b' to go back");
    }
    
    int addedStocks;
    double additionalCosts;
    char itemId[ID_LENGTH];
    char messageWithName[100];
    struct Node *item = NULL;

    // ask for id
    scanf("%s", itemId);

    bool isTryingToGoBack = strcmp(itemId, "b") == 0 || strcmp(itemId, "B") == 0;

    if (isTryingToGoBack || listIsEmpty) return;

    // get the item based on id
    item = item_getItemById(head, itemId);

    // if we didn't find the item, give them an error
    if (item == NULL) {
        display_newUserMessagePage("Restocking an Item", "", "Error: Item does not exist. Please try again", "", "", "", "");
        sleep(SLEEP_TIME);
        return;
    }

    // constructing the message to show on the page for the user
    sprintf(messageWithName, "Enter the amount of stocks added in %s", item->data.name);
    display_itemDataPromptPage(item->data, "Restocking an Item", messageWithName, "");
    scanf("%d", &addedStocks);

    // TODO: ask the user for costs again, but have an option to use the previous costs

    // display_itemDataPromptPage(item->data, "Restocking an Item", "Enter the original price of each item to calculate its costs", "Enter '1' to use the previous costs");
    // display_itemDataPromptPage(item->data, "Restocking an Item", "Enter the original price of each item to calculate its costs", "Enter '0' to use the previous costs");
    // char costs[10];
    // scanf("%s", costs);
    // atoi()
    // if (strcmp())

    // update the stocks and base stocks
    item->data.stocks += addedStocks;
    if (item->data.stocks > item->data.baseStocks) item->data.baseStocks = item->data.stocks;

    display_newUserMessagePage("Restocking an Item", "", "Enter additional costs if there are any:", "", "", "", "");
    scanf("%lf", &additionalCosts);

    // tally the costs
    sales_updateCosts(monthlyProfits, addedStocks, item->data.originalPrice);
    sales_updateAdditionalCosts(monthlyProfits, additionalCosts);
    
    // update the data from storage
    sales_updateReportsFromStorage(monthlyProfits);
    storage_editItemFromStorageById(item->data.id, item->data);

    utils_updateDate(item->data.lastUpdated);
    display_newUserMessagePage("Restocking an Item", "", "Item restocked successfully!", "", "", "", "");
    sleep(SLEEP_TIME);
}

// handles the searching of an item
void item_searchItemHandler(struct Node **list)
{
    bool listIsEmpty = *list == NULL;
    
    if (listIsEmpty) {
        display_newUserMessagePage("Searching an Item", "Enter any key to go back", "No item to search.", "", "", "", "");
    } else {
        display_newUserMessagePage("Searching an Item", "Enter 'b' to go back.", "Enter a keyword to search: ", "", "", "", "");
    }

    char searchTerm[NAME_SIZE];
    // create a current variable to prevent the head pointer from being manipulated
    struct Node *current = *list;
    // another linked list to store the results
    struct Node *results = NULL;

    fflush(stdin);
    fgets(searchTerm, NAME_SIZE, stdin);
    utils_clearNewline(searchTerm);

    bool isTryingToGoBack = strcmp(searchTerm, "b") == 0 || strcmp(searchTerm, "B") == 0;

    if (isTryingToGoBack || listIsEmpty) return;

    // we traverse the list and on each iteration, check if searchTerm is a substring of the current data name
    while(current != NULL) {
        // strstr() is a function in <string.h> that will check if a string is a substring of another string. in simple terms, if it fits the searchTerm
        if(strstr(current->data.name, searchTerm) != NULL) 
            // if it is a substring, add it to the results
            item_addItemToList(&results, current->data);
        
        // go to next node (iterator)
        current = current->next;
    }

    // render the results
    system("cls");
    display_inventoryPromptPage(&results, "", "Enter any key to go back.");
    // free the memory of results
    utils_freeLinkedList(&results);

    // to be able to go back to inventory page
    getch();
}

// handles the process of prompting the user for an item to be added
void item_addItemHandler(struct Node **head, struct ReportPerMonth monthlyProfits[], char categories[][CATEGORY_NAME_LEN], int *categoriesLen)
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

    display_newUserMessagePage("Adding an Item", "Enter 'b' to go back", message[0], "", "", "", "");
    fflush(stdin);
    fgets(newItem.name, NAME_SIZE, stdin);
    utils_clearNewline(newItem.name);

    bool isTryingToGoBack = strcmp(newItem.name, "b") == 0 || strcmp(newItem.name, "B") == 0;
    if (isTryingToGoBack) return;

    bool nameTooShort = strlen(newItem.name) <= 1;
    if (nameTooShort) {
        display_newUserMessagePage("Adding an Item", "Enter 'b' to go back", "Items with no name are not allowed.", "", "", "", "");
        sleep(SLEEP_TIME);
        return;
    }

    // TODO: fix this?
    bool nameTooLong = strlen(newItem.name) > NAME_SIZE - 1;
    if (nameTooLong) {
        display_newUserMessagePage("Adding an Item", "Enter 'b' to go back", "Item name is too long, please try again.", "", "", "", "");
        sleep(SLEEP_TIME);
        return;
    }

    // ask the user for the category
    category_itemCategoryPrompter(newItem.category, categories, categoriesLen);

    display_newUserMessagePage("Adding an Item", "", message[1], "", "", "", "");
    scanf("%d", &newItem.stocks);
    newItem.baseStocks = newItem.stocks;

    display_newUserMessagePage("Adding an Item", "", message[2], "", "", "", "");
    scanf("%lf", &newItem.price);

    display_newUserMessagePage("Adding an Item", "", message[3], "", "", "", "");
    scanf("%lf", &newItem.originalPrice);

    display_newUserMessagePage("Adding an Item", "", message[4], "", "", "", "");
    scanf("%lf", &additionalCosts);
    
    // after getting all the necessary data from user, add it to the list
    item_addItemMetaData(head, monthlyProfits, newItem.name, newItem.stocks, newItem.price, newItem.originalPrice, newItem.category, additionalCosts);
    display_newUserMessagePage("Adding an Item", "", "Item added succesfully!", "", "", "", "");
    sleep(SLEEP_TIME);
}

// handles the process of deleting an item on the inventory based on ID
void item_deleteItemHandler(struct Node **head, struct ReportPerMonth monthlyProfits[])
{
    bool listIsEmpty = *head == NULL;

    if (listIsEmpty) {
        display_newUserMessagePage("Deleting an Item", "Enter any key to go back", "No item to delete.", "", "", "", "");
    } else {
        display_inventoryPromptPage(head, "Enter the ID of the item you want to delete:", "Enter 'b' to go back");
    }

    // get the ID to be deleted
    char idToDelete[ID_LENGTH];
    // we keep track of the deleted Node to free its memory since it was dynamically allocated
    struct Node *deleted = NULL;
    // we create a current variable so that we don't manipulate the pointer of the head
    struct Node *current = *head;

    scanf("%s", idToDelete);

    bool isTryingToGoBack = strcmp(idToDelete, "b") == 0 || strcmp(idToDelete, "B") == 0;
    
    if (isTryingToGoBack || listIsEmpty) return;

    // if the user is trying to delete the first item, we simply set the head to point to the next node
    if (strcmp(current->data.id, idToDelete) == 0) {
        deleted = *head;

        // ask the user if he/she wants to deduct the total costs of the deleted item
        double deduction = deleted->data.originalPrice * (double)deleted->data.stocks;
        sales_reflectToMonthlyCostsOnDeletion(monthlyProfits, deduction);

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
            display_newUserMessagePage("Deleting an Item", "", "Error: Item does not exist. Please try again.", "", "", "", "");
            sleep(SLEEP_TIME);
            return;
        }

        deleted = current->next;

        // ask the user if he/she wants to deduct the total costs of the deleted item
        double deduction = deleted->data.originalPrice * (double)deleted->data.stocks;
        sales_reflectToMonthlyCostsOnDeletion(monthlyProfits, deduction);

        // delete it by overwritting it
        if (current->next->next == NULL) current->next = NULL;
        else current->next = current->next->next;
    }

    // delete it from the storage as well
    storage_deleteItemFromStorageById(idToDelete);
    display_newUserMessagePage("Deleting an Item", "", "Item deleted succesfully!", "", "", "", "");
    // free the memory
    free(deleted);
    sleep(SLEEP_TIME);
}

// handles the editing of an item
void item_editItemHandler(struct Node **head, char categories[][CATEGORY_NAME_LEN], int *categoriesLen)
{
    bool listIsEmpty = *head == NULL;

    if (listIsEmpty) {
        display_newUserMessagePage("Editing an Item", "Enter any key to go back", "No item to edit.", "", "", "", "");
    } else {
        display_inventoryPromptPage(head, "Enter the ID of the item you want to edit:", "Enter 'b' to go back");
    }

    // the options to edit
    int toUpdate;
    // this is just to dynamically show a message with the name of item
    char headerWithName[100];
    // this will hold the index of item to be deleted
    char itemId[ID_LENGTH];
    scanf("%s", itemId);

    bool isTryingToGoBack = strcmp(itemId, "b") == 0 || strcmp(itemId, "B") == 0;

    if (isTryingToGoBack || listIsEmpty) return;

    // get the item
    struct Node *item = item_getItemById(head, itemId);

    // if we didn't find the item, give them an error
    if (item == NULL) {
        display_newUserMessagePage("Editing an Item", "", "Error: Item does not exist. Please try again", "", "", "", "");
        sleep(SLEEP_TIME);
        return;
    }

    display_itemDataPromptPage(item->data, "Editing an Item", "[ 1. name | 2. category | 3. selling price | 4. original price | 5. base stocks ]", "What do you want to edit?");
    // display_newUserMessagePage("Editing an Item", "", "Options:", "[ 1. name | 2. category | 3. selling price | 4. original price | 5. base stocks ]", "What do you want to edit?: ", "", "");
    scanf("%d", &toUpdate);

    // after that, we simply determine what the user wants to update and prompt the user for new data
    switch(toUpdate) 
    {
    // EDITED: NAME
    case 1:
        strcpy(headerWithName, "Enter a new name for ");
        strcat(headerWithName, item->data.name);
        display_itemDataPromptPage(item->data, "Editing an Item", headerWithName, "");

        fflush(stdin);
        fgets(item->data.name, NAME_SIZE, stdin);
        utils_clearNewline(item->data.name);
        break;
    // EDITED: category
    case 2:
        category_itemCategoryPrompter(item->data.category, categories, categoriesLen);
        break;
    // EDITED: selling price
    case 3:
        strcpy(headerWithName, "Enter an updated selling price of ");
        strcat(headerWithName, item->data.name);
        display_itemDataPromptPage(item->data, "Editing an Item", headerWithName, "");

        scanf("%lf", &item->data.price);
        item->data.profit = item->data.price - item->data.originalPrice;
        break;
    // EDITED: original price
    case 4:
        strcpy(headerWithName, "Enter an updated original price of ");
        strcat(headerWithName, item->data.name);
        display_itemDataPromptPage(item->data, "Editing an Item", headerWithName, "");
        
        scanf("%lf", &item->data.originalPrice);
        item->data.profit = item->data.price - item->data.originalPrice;
        break;
    // EDITED: base stocks
    case 5:
        strcpy(headerWithName, "Enter an updated base stocks of ");
        strcat(headerWithName, item->data.name);
        display_itemDataPromptPage(item->data, "Editing an Item", headerWithName, "");

        scanf("%d", &item->data.baseStocks);
        break;
    default:
        display_newUserMessagePage("Editing an Item", "", "Incorrect number entered", "", "", "", "");
        return;
    }

    utils_updateDate(item->data.lastUpdated);
    // update the item in the storage
    storage_editItemFromStorageById(itemId, item->data);

    display_newUserMessagePage("Editing an Item", "", "Item edited succesfully!", "", "", "", "");
    sleep(SLEEP_TIME);
}

// handles the process of viewing more details of an item by asking for ID
void item_viewItemDetailsHandler(struct Node **head)
{
    bool listIsEmpty = *head == NULL;

    if (listIsEmpty){
        display_newUserMessagePage("Viewing an Item", "Enter any key to go back", "No item to view.", "", "", "", "");
    } else {
        display_inventoryPromptPage(head, "Enter the ID of the item you want to view:", "Enter 'b' to go back");
    }

    char itemId[ID_LENGTH];
    scanf("%s", itemId);

    bool isTryingToGoBack = strcmp(itemId, "b") == 0 || strcmp(itemId, "B") == 0;

    if (isTryingToGoBack || listIsEmpty) return;

    // get the item
    struct Node *itemData = item_getItemById(head, itemId);

    if (itemData == NULL) {
        display_newUserMessagePage("Viewing an Item", "", "Item not found, please try again.", "", "", "", "");
        sleep(SLEEP_TIME);
        return;
    }

    // render it if it is found
    system("cls");
    display_itemDataPage(itemData->data);

    // to be able to go back to inventory page
    char x;
    bannerUserInput();
    fflush(stdin);
    scanf("%c", &x);
}

void item_changeSortingHandler(struct Node **head)
{
    if (*head == NULL) {
        display_newUserMessagePage("Sorting Items", "", "No items to sort.", "", "", "", "");
        sleep(SLEEP_TIME);
        return;
    } else {
        display_inventoryPromptPage(head, "0. Default | 1. Low Stock | 2. High Stocks | 3. Low Price | 4. High Price | 5. Low Profit | 6. High Profit", "Sort items by:");
    }

    int userInput;
    scanf("%d", &userInput);

    // if the input is out of bounds, don't proceed
    if (userInput < 0 || userInput > 6) {
        display_newUserMessagePage("Sorting Items", "", "Invalid Input.", "", "", "", "");
        sleep(SLEEP_TIME);
        return;
    }

    settings.sortBy = userInput;
    storage_updateSettingsFromStorage();
}

// gives you the item based on ID, returns NULL if not found
struct Node *item_getItemById(struct Node **list, char itemId[])
{
    // base case, if we reached the end and still haven't fount the item, return NULL
    if (*list == NULL) return NULL;

    struct Node *currentItem = *list;

    // if the currentItem item is equal to the given ID, return it
    if (strcmp(currentItem->data.id, itemId) == 0) return currentItem;

    // recursive call
    return item_getItemById(&currentItem->next, itemId);
}

// handles the process of setting the data and tallying the costs, records of an item being added
void item_addItemMetaData(struct Node **head, struct ReportPerMonth monthlyProfits[], char name[], int stocks, double price, double originalPrice, char category[], double additionalCost)
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

    utils_updateDate(newItem.dateAdded);
    utils_updateDate(newItem.lastUpdated);

    utils_generateId(newItem.id);

    // tally the costs
    sales_updateCosts(monthlyProfits, newItem.stocks, newItem.originalPrice);
    sales_updateAdditionalCosts(monthlyProfits, additionalCost);
    // update the storage
    sales_updateReportsFromStorage(monthlyProfits);

    item_addItemToList(head, newItem);
    storage_addItemToStorage(newItem);
}

// handles the actual addition to the list
// REMINDER: use the function utils_freeLinkedList() after using this funciton because it dynamically allocates memory
void item_addItemToList(struct Node **head, struct Item item)
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

// updates the category of the affected items in both the list and file storage. could be optimized but im not willing to write any more code
void item_updateItemsCategory(struct Node **head, char *oldCategory, char *newCategory)
{
    // base case, this indicates that we've reached the end
    if (*head == NULL) return;

    struct Node *currentItem = *head;

    if (strcmp(currentItem->data.category, oldCategory) == 0) {
        strcpy(currentItem->data.category, newCategory);
        storage_editItemFromStorageById(currentItem->data.id, currentItem->data);
    }

    // recursive call
    item_updateItemsCategory(&currentItem->next, oldCategory, newCategory);
}

// gets the items with the given category and appends it in your desired placeholder
void item_getItemsByCategory(struct Node **head, char *category, struct Node **placeholder)
{
    // base case
    if (*head == NULL) return;

    struct Node *currentItem = *head;

    if (strcmp(currentItem->data.category, category) == 0) item_addItemToList(placeholder, currentItem->data);

    // recursive call
    item_getItemsByCategory(&currentItem->next, category, placeholder);
}

void item_getStockStatus(char *status, int stocks, int baseStocks)
{
    double percentage = utils_getPercentage(stocks, baseStocks);

    if (percentage == 100) strcpy(status, "Full Stock");
    else if (percentage <= 99.0 && percentage >= 35.0) strcpy(status, "In Stock");
    else if (percentage <= 34.0 && percentage >= 0.1) strcpy(status, "Low Stock");
    else strcpy(status, "No Stock");
}

void item_determineWhatToSort(char *placeholder)
{
    switch (settings.sortBy)
    {
    case 1:
        strcpy(placeholder, "Low Stock");
        break;
    case 2:
        strcpy(placeholder, "High Stock");
        break;
    case 3:
        strcpy(placeholder, "Low Price");
        break;
    case 4:
        strcpy(placeholder, "High Price");
        break;
    case 5:
        strcpy(placeholder, "Low Profit");
        break;
    case 6:
        strcpy(placeholder, "High Profit");
        break;
    case 0:
        strcpy(placeholder, "Default");
        break;
    }
}

void item_getSortedItems(struct Node **head, struct Node **output)
{
    if (*head == NULL) return;

    utils_freeLinkedList(output);
    
    switch (settings.sortBy)
    {
    case 1:
        item_sortItemsBy(item_isStocksLessThan, head, output);
        break;
    case 2:
        item_sortItemsBy(item_isStocksGreaterThan, head, output);
        break;
    case 3:
        item_sortItemsBy(item_isPriceLessThan, head, output);
        break;
    case 4:
        item_sortItemsBy(item_isPriceGreaterThan, head, output);
        break;
    case 5:
        item_sortItemsBy(item_isProfitLessThan, head, output);
        break;
    case 6:
        item_sortItemsBy(item_isProfitGreaterThan, head, output);
        break;
    case 0:
        *output = *head;
        break;
    }
}

// gives a sorted output list of a given original list based on a callback function passed into it
void item_sortItemsBy(bool (*callbackFn)(struct Item, struct Item), struct Node **head, struct Node **output)
{
    // head is the source, output is where the sorted list goes
    struct Node *current = *head;

    // loop over the source(head)
    while (current != NULL) {
        struct Node *currentOutput = *output;
        int index = 0;

        // if it's the first item, just add it directly
        if (currentOutput == NULL) {
            // create a new  block of memory forthe output list so that it doesn't disappear
            struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
            newNode->data = current->data;
            newNode->next = NULL;

            // add it to the output list
            *output = newNode;

            // iterate the next before continueing
            current = current->next;
            continue;
        }

        // loop over the output and compare the current source item to the current output item
        while (currentOutput != NULL) {
            // the condition depends on the callback function passed
            if (callbackFn(current->data, currentOutput->data)) {
                item_insertItemAt(index, output, &current);
                break;
            }

            index++;
            currentOutput = currentOutput->next;
        }

        // if its the last item, insert it at the end
        if (currentOutput == NULL) {
            item_insertItemAt(index, output, &current);
        }

        current = current->next;
    }
}

// CALLBACK FUNCTION FOR SORTING OPTIONS

bool item_isStocksLessThan(struct Item item1, struct Item item2)
{
    if (utils_getPercentage(item1.stocks, item1.baseStocks) < utils_getPercentage(item2.stocks, item2.baseStocks)) return true;
    return false;
}

bool item_isStocksGreaterThan(struct Item item1,struct Item item2)
{
    if (utils_getPercentage(item1.stocks, item1.baseStocks) > utils_getPercentage(item2.stocks, item2.baseStocks)) return true;
    return false;
}

bool item_isPriceLessThan(struct Item item1, struct Item item2)
{
    if (item1.price < item2.price) return true;
    return false;
}

bool item_isPriceGreaterThan(struct Item item1,struct Item item2)
{
    if (item1.price > item2.price) return true;
    return false;
}

bool item_isProfitLessThan(struct Item item1, struct Item item2)
{
    if (item1.profit < item2.profit) return true;
    return false;
}

bool item_isProfitGreaterThan(struct Item item1,struct Item item2)
{
    if (item1.profit > item2.profit) return true;
    return false;
}

// END

void item_insertItemAt(int index, struct Node **destination, struct Node **toInsert)
{
    struct Node *current = *destination;
    struct Node *nodeToInsert = *toInsert;

    // create a new  block of memory forthe output list so that it doesn't disappear
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    newNode->data = nodeToInsert->data;
    newNode->next = NULL;

    // if we're trying to insert it first
    if (index == 0) {
        newNode->next = *destination;
        *destination = newNode;
        return;
    }

    // traverse to the index
    for (int i = 0; i < index - 1; i++) {
        // if we're trying to add it in the end
        if (current->next == NULL) {
            current->next = newNode;
            return;
        }
        // otherwise, continue traversing
        current = current->next;
    }

    // save the rest of the list
    struct Node *rest = current->next;
    // add the node to be inserted
    current->next = newNode;

    newNode->next = rest;
}

void item_regenerateItemIdList(struct Node **head)
{
    struct Node *current = *head;
    char oldID[ID_LENGTH];

    while (current != NULL) {
        strcpy(oldID, current->data.id);
        utils_generateId(current->data.id);

        storage_editItemFromStorageById(oldID, current->data);

        current = current->next;
    }
}


// void sortItemsByLowStocks(struct Node **head, struct Node **output)
// {
//     // head is the source, output is where the sorted list goes
//     struct Node *current = *head;

//     // loop over the source(head)
//     while (current != NULL) {
//         struct Node *currentOutput = *output;
//         int index = 0;

//         // if it's the first item, just add it directly
//         if (currentOutput == NULL) {
//             // create a new  block of memory forthe output list so that it doesn't disappear
//             struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
//             newNode->data = current->data;
//             newNode->next = NULL;

//             *output = newNode;

//             // iterate the next before continueing
//             current = current->next;
//             continue;
//         }

//         // loop over the output and compare the current source item to the current output item
//         while (currentOutput != NULL) {
//             // if the current source item's stock is lower than the current output item's stock, insert it before that current output item
//             if (utils_getPercentage(current->data.stocks, current->data.baseStocks) < utils_getPercentage(currentOutput->data.stocks, currentOutput->data.baseStocks)) {
//                 item_insertItemAt(index, output, &current);
//                 break;
//             }

//             index++;
//             currentOutput = currentOutput->next;
//         }

//         // if its the last item, insert it at the end
//         if (currentOutput == NULL) {
//             item_insertItemAt(index, output, &current);
//         }

//         current = current->next;
//     }
// }