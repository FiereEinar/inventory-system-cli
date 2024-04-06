#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "main.h"

void sellItem(struct Node **head, struct ProfitPerMonth monthlyProfits[])
{
    system("cls");
    struct Node *current = *head;
    char itemId[ID_LENGTH];
    double quantity;
    int updated = 0;

    printf("\nType [x] to exit.");
    printf("\n\nEnter the item ID: \n>>> ");
    scanf("%s", itemId);

    if (strcmp(itemId, "x") == 0 || strcmp(itemId, "X") == 0)
    {
        printActions();
        return;
    }
    printf("Enter the quantity purchased: \n>>> ");
    scanf("%lf", &quantity);

    // find the item in the list
    while (current != NULL) {
        if (strcmp(current->data.id, itemId) == 0) {
            // if there's not enough stocks, don't proceed
            if (current->data.stocks < quantity)
            {
                printf("\nNot enough stocks to fulfill the purchase.");
                sleep(SLEEP_TIME);
                return;
            }

            // once the item is found, update the stocks and tally the profit and revenue
            current->data.stocks -= (int)quantity;
            monthlyProfits[getCurrentDateInt()].profit += quantity * (current->data.price - current->data.originalPrice);
            monthlyProfits[getCurrentDateInt()].revenue += quantity * current->data.price;
            updated = 1;
            break; // exit the loop once item is found and updated
        }
        current = current->next;
    }

    if (updated == 0)
    {
        printf("\nItem not found");
        sleep(SLEEP_TIME);
        return;
    }

    printLinkedlist(*head);
    printf("\n\nPurchase deducted to stocks successfully!");
    printf("\n\nEnter 'b' to go back.");
}

void restockItem(struct Node **head, struct ProfitPerMonth monthlyProfits[])
{
    printf("Restocking an item.");

    if (*head == NULL)
    {
        printf("\nNo item to restock.");
        sleep(SLEEP_TIME);
        return;
    }
    
    int addedStocks;
    double additionalCosts;
    char idToDelete[ID_LENGTH];

    // prompt the user for some details
    printf("\n\nEnter the ID of the item: \n>>> ");
    scanf("%s", idToDelete);

    // we create a current variable so that we don't manipulate the pointer of the head
    struct Node *current = *head;
    // traverse to the Node/item that the user wants to update
    while(current->next != NULL)
    {
        if (strcmp(current->data.id, idToDelete) == 0) break;
        current = current->next;
    }

    // if we didn't find the item, give them an error
    if (strcmp(current->data.id, idToDelete) != 0)
    {
        printf("\nError: Item does not exist. Please try again");
        sleep(SLEEP_TIME);
        return;
    }

    printf("\nEnter the amount of stocks added in %s: \n>>> ", current->data.name);
    scanf("%d", &addedStocks);
    current->data.stocks += addedStocks;
    current->data.baseStocks = current->data.stocks;

    printf("Any additional costs? (gas, transportation, etc.) \n>>> ");
    scanf("%lf", &additionalCosts);

    // tally the costs
    monthlyProfits[getCurrentDateInt()].costs += (addedStocks * current->data.originalPrice) + additionalCosts;
    // update the lastUpdated data
    sprintf(current->data.lastUpdated, "%s / %s", getCurrentDate(), getCurrentTime());
}

struct Node *getItemById(struct Node **list, char itemId[])
{
    struct Node *current = *list;

    while (current != NULL)
    {
        if (strcmp(current->data.id, itemId) == 0)
            return current;
        
        current = current->next;
    }
    return NULL;
}

void searchItem(struct Node **list)
{
    printf("Searching an item.");

    if (*list == NULL)
    {
        printf("\nNo item to search.");
        printf("\nEnter 'b' to go back.");
        return;
    }

    char searchTerm[20];
    struct Node *current = *list;
    // another linked list to store the results
    struct Node *results = NULL;
    // tracker for the tail
    struct Node *last = NULL;

    // TODO: maybe use fgets()
    printf("\n\nEnter a keyword: ");
    scanf("%s", searchTerm);

    printf("\nSearching for: %s\n", searchTerm);

    // we traverse the list and on each iteration, check if searchTerm is a substring of the current data name
    while(current != NULL) {
        // strstr() is a function in <string.h> that will check if a string is a substring of another string. in simple term, if it fits the searchTerm
        if(strstr(current->data.name, searchTerm) != NULL) {
            // if true we create a new instance of Node with the same data
            struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
            newNode->data = current->data;
            newNode->next = NULL;

            // then store it in the results
            // this checks if this is the first element being stored
            if(results == NULL) {
                results = newNode;
                // we keep track of the tail of the list so that we don't have to loop our way there
                last = results;
            } else {
                // if its not the first Node, go to the tail (last) and add it there
                last->next = newNode;
                // keep track of tail
                last = newNode;
            }
        }
        // go to next node (iterator)
        current = current->next;
    }
    system("cls");
    // print all the result
    inventoryPage(&results);
    // free the memory of results
    freeLinkedList(&results);
}

void addItem(struct Node **head, struct ProfitPerMonth monthlyProfits[])
{
    printf("Adding an item.\n\n");
    // using item struct to store values, less variable declaration needed
    struct Item newItem;
    double additionalCosts;

    // prompt the user with all the data and store it in that newItem structure
    fflush(stdin);
    printf("Enter the name of item: \n>>> ");
    // use fgets to allow string with spaces
    fgets(newItem.name, NAME_SIZE, stdin);
    // clear the newline character from fgets()
    size_t len = strlen(newItem.name);
    if (len > 0 && newItem.name[len - 1] == '\n')
        newItem.name[len - 1] = '\0';

    printf("Enter the stocks of item: \n>>> ");
    scanf("%d", &newItem.stocks);
    newItem.baseStocks = newItem.stocks;

    printf("Enter the selling price of item: \n>>> ");
    scanf("%lf", &newItem.price);

    printf("Enter the original price of item: \n>>> ");
    scanf("%lf", &newItem.originalPrice);

    printf("Any additional costs? (gas, transportation, etc.) \n>>> ");
    scanf("%lf", &additionalCosts);
    
    // after getting all the necessary data from user, add it to the list
    addItemToList(head, monthlyProfits, newItem.name, newItem.stocks, newItem.price, newItem.originalPrice, additionalCosts);
}

void deleteItem(struct Node **head, struct ProfitPerMonth monthlyProfits[])
{
    printf("Deleting an item.");
    if (*head == NULL)
    {
        printf("\nNo item to delete.");
        sleep(SLEEP_TIME);
        return;
    }
    // this will hold the index of item to be deleted
    char idToDelete[ID_LENGTH];

    printf("\n\nEnter the ID of the item you want to delete: ");
    bannerUserInput();
    scanf("%s", idToDelete);

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
        if (current->next == NULL)
        {
            printf("\nError: Item does not exist. Please try again");
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
    // free the memory
    free(deleted);
}

// crazy function
void reflectToMonthlyCostsOnDeletion(struct ProfitPerMonth monthlyProfits[], double deduction)
{
    char action;

    printf("Do you want to deduct the total costs of deleted item to your monthly costs report?[y/n]\n>>> ");
    fflush(stdin);
    scanf("%c", &action);

    if (action == 'y' || action == 'Y') {
        monthlyProfits[getCurrentDateInt()].costs -= deduction;
        printf("\nCosts deducted.");
    }
}

void editItem(struct Node **head)
{
    printf("Editing an item.");

    if (*head == NULL)
    {
        printf("\nNo item to edit.");
        sleep(SLEEP_TIME);
        return;
    }

    int toUpdate;
    // this will hold the index of item to be deleted
    char idToDelete[ID_LENGTH];

    // prompt the user for some details
    printf("\n\nEnter the ID of the item: \n>>> ");
    scanf("%s", idToDelete);

    printf("Options: [ 1. name | 2. original price | 3. selling price ]\n");
    printf("What do you want to edit?: \n>>> ");
    scanf("%d", &toUpdate);

    // we create a current variable so that we don't manipulate the pointer of the head
    struct Node *current = *head;
    // traverse to the Node/item that the user wants to update
    while(current->next != NULL)
    {
        if (strcmp(current->data.id, idToDelete) == 0) break;
        current = current->next;
    }

    // if we didn't find the item, give them an error
    if (strcmp(current->data.id, idToDelete) != 0)
    {
        printf("\nError: Item does not exist. Please try again");
        sleep(SLEEP_TIME);
        return;
    }

    // after that, we simply determine what the user wants to update and prompt the user for new data
    switch(toUpdate) {
        case 1:
            printf("Enter a new name of %s: \n>>> ", current->data.name);
            fflush(stdin);
            fgets(current->data.name, NAME_SIZE, stdin);

            // clear the newline character from fgets()
            size_t len = strlen(current->data.name);
            if (len > 0 && current->data.name[len - 1] == '\n')
                current->data.name[len - 1] = '\0';
            break;
        case 2:
            printf("Enter an updated original price of %s(Original: %.2lf): \n>>> ", current->data.name, current->data.originalPrice);
            scanf("%lf", &current->data.originalPrice);
            current->data.profit = current->data.price - current->data.originalPrice;
            break;
        case 3:
            printf("Enter an updated selling price of %s: \n>>> ", current->data.name);
            scanf("%lf", &current->data.price);
            current->data.profit = current->data.price - current->data.originalPrice;
            break;
        default:
            printf("Error: Incorrect number entered.");
            break;
    }

    // update the lastUpdated data
    sprintf(current->data.lastUpdated, "%s / %s", getCurrentDate(), getCurrentTime());
}

void addItemToList(struct Node **head, struct ProfitPerMonth monthlyProfits[], char name[], int stocks, double price, double originalPrice, double additionalCost)
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

    sprintf(newItem.dateAdded, "%s / %s", getCurrentDate(), getCurrentTime());
    sprintf(newItem.lastUpdated, "%s / %s", getCurrentDate(), getCurrentTime());

    generateId(newItem.id);
    // tally the costs
    monthlyProfits[getCurrentDateInt()].costs += (newItem.stocks * newItem.originalPrice) + additionalCost;

    // create a dynamically allocated node 
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    newNode->data = newItem;
    newNode->next = NULL;

    // the logic for adding it to the list
    if (*head == NULL) {
        *head = newNode;
    } else {
        struct Node *current = *head;
        while(current->next != NULL)
            current = current->next;
        
        current->next = newNode;
    }
}