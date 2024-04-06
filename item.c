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
    printf("\n\nEnter the quantity purchased: \n>>> ");
    scanf("%lf", &quantity);

    // find the item in the list
    while (current != NULL) {
        if (strcmp(current->data.id, itemId) == 0) {
            // if there's not enough stocks, don't proceed
            if (current->data.stocks < quantity)
            {
                printf("\nNot enough stocks to fulfill the purchase.");
                return;
            }


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
        return;
    }

    printLinkedlist(*head);
    printf("\n\nPurchase deducted to stocks successfully!");
    printf("\n\nEnter 'b' to go back.");
}

void restockItem(struct Node **head, struct ProfitPerMonth monthlyProfits[])
{
    system("cls");
    printf("Restocking an item.\n\n");
    printLinkedlist(*head);
    if (*head == NULL)
    {
        printf("\nNo item to restock.");
        printf("\nEnter 'b' to go back.");
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
        return;
    }

    printf("\nEnter the amount of stocks added in %s: \n>>> ", current->data.name);
    scanf("%d", &addedStocks);
    current->data.stocks += addedStocks;
    current->data.baseStocks = current->data.stocks;

    printf("\nAny additional costs? (gas, transportation, etc.) \n>>> ");
    scanf("%lf", &additionalCosts);

    monthlyProfits[getCurrentDateInt()].costs += (addedStocks * current->data.originalPrice) + additionalCosts;

    printLinkedlist(*head);
    printf("\n\nStocks added succesfully!");
    printf("\nEnter 'b' to go back.");
}

void searchItem(struct Node **list)
{
    system("cls");
    printf("Searching an item.\n\n");
    printLinkedlist(*list);
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

    printf("\nSearch: %s", searchTerm);

    // we traverse the list and on each iteration, check if searchTerm is a substring of the current data name
    while(current != NULL) {
        // strstr() is a function in <string.h> that will check if a string is a substring of another string. AKA if it fits the searchTerm
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
    // print all the result
    printLinkedlist(results);
    // free the memory of results
    freeLinkedList(&results);
    printf("\n\nEnter 'b' to go back.");
}

void addItem(struct Node **head, struct ProfitPerMonth monthlyProfits[])
{
    system("cls");
    printf("Adding an item.\n\n");
    // create a new instance of the Item struct
    struct Item newItem;
    double additionalCosts;

    fflush(stdin);
    // prompt the user with all the data and store it in that newItem structure
    printf("Enter the name of item: \n>>> ");
    // use fgets to allow string with spaces
    fgets(newItem.name, NAME_SIZE, stdin);

    // clear the newline character from fgets()
    size_t len = strlen(newItem.name);
    if (len > 0 && newItem.name[len - 1] == '\n')
        newItem.name[len - 1] = '\0'; // replace newline with null character
    //scanf("%s", newItem.name);

    printf("Enter the stocks of item: \n>>> ");
    scanf("%d", &newItem.stocks);
    newItem.baseStocks = newItem.stocks;

    printf("Enter the selling price of item: \n>>> ");
    scanf("%lf", &newItem.price);

    printf("Enter the original price of item: \n>>> ");
    scanf("%lf", &newItem.originalPrice);

    printf("\nAny additional costs? (gas, transportation, etc.) \n>>> ");
    scanf("%lf", &additionalCosts);

    // get the current date and time
    sprintf(newItem.dateAdded, "%s | %s", getCurrentDate(), getCurrentTime());
    sprintf(newItem.lastUpdated, "%s | %s", getCurrentDate(), getCurrentTime());

    generateId(newItem.id);
    // tally the cost
    monthlyProfits[getCurrentDateInt()].costs += (newItem.stocks * newItem.originalPrice) + additionalCosts;
    newItem.profit = newItem.price - newItem.originalPrice;

    // we then create a new Node structure, it is dynamically allocated in the memory
    // so that it doesn't get cleared when this function exits.
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    // here we just set the data property of the Node to our Item struct
    newNode->data = newItem;
    // the next will be null since this new Item Node will be added at the end of the list
    newNode->next = NULL;

    // if this is the first item that is added then we simply set the head to our newly created Node
    if (*head == NULL) {
        *head = newNode;
    // otherwise we need to traverse the end of the list and add it there
    } else {
        // TODO: create a tail in main() so that we don't have to traverse our way to the tail
        // here we create a (current) variable to prevent our head from being manipulated
        struct Node *current = *head;
        // it is guaranteed that at the end of the list, its next property will always be NULL
        // so we use it to check wether or not we have reached the end of the list
        while(current->next != NULL)
        {
            // we traverse the list by simply setting our current pointer to point to the next Node
            // this will continue until the next property in NULL and that means we have reach the end
            current = current->next;
        }
        // after we reached the end, we simply set the next pointer
        // (that was recently pointing to NULL) to point to our newly created Node
        current->next = newNode;
    }
    printLinkedlist(*head);
    printf("\n\nItem added successfully!");
    printf("\nEnter 'b' to go back.");
}

void deleteItem(struct Node **head, struct ProfitPerMonth monthlyProfits[])
{
    system("cls");
    printf("Deleting an item.\n\n");
    printLinkedlist(*head);
    if (*head == NULL)
    {
        printf("\nNo item to delete.");
        printf("\nEnter 'b' to go back.");
        return;
    }
    // this will hold the index of item to be deleted
    char idToDelete[ID_LENGTH];

    printf("\n\nEnter the ID of the item you want to delete: ");
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
    // after deleting the item, reprint the list
    printLinkedlist(*head);
    printf("\n\nItem deleted successfully!");
    printf("\nEnter 'b' to go back.");
}

// crazy function
void reflectToMonthlyCostsOnDeletion(struct ProfitPerMonth monthlyProfits[], double deduction)
{
    char action;

    printf("\nD you want to deduct the total costs of deleted item to your monthly costs report?[y/n]\n>>> ");
    fflush(stdin);
    scanf("%c", &action);

    if (action == 'y' || action == 'Y') {
        monthlyProfits[getCurrentDateInt()].costs -= deduction;
        printf("\nCosts deducted.");
    }
}

void editItem(struct Node **head)
{
    system("cls");
    printf("Editing an item.\n\n");
    printLinkedlist(*head);
    if (*head == NULL)
    {
        printf("\nNo item to edit.");
        return;
    }

    int toUpdate;
    // this will hold the index of item to be deleted
    char idToDelete[ID_LENGTH];

    // prompt the user for some details
    printf("\n\nEnter the ID of the item: \n>>> ");
    scanf("%s", idToDelete);

    printf("\nOptions: [ 1. name | 2. original price | 3. selling price ]");
    printf("\nWhat do you want to update?: \n>>> ");
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
        return;
    }

    // after that, we simply determine what the user wants to update and prompt the user for new data
    switch(toUpdate) {
        case 1:
            printf("\nEnter a new name of %s: \n>>> ", current->data.name);
            fflush(stdin);
            fgets(current->data.name, NAME_SIZE, stdin);

            // clear the newline character from fgets()
            size_t len = strlen(current->data.name);
            if (len > 0 && current->data.name[len - 1] == '\n')
                current->data.name[len - 1] = '\0';
            break;
        case 2:
            printf("\nEnter an updated original price of %s(Original: %.2lf): \n>>> ", current->data.name, current->data.originalPrice);
            scanf("%lf", &current->data.originalPrice);
            current->data.profit = current->data.price - current->data.originalPrice;
            break;
        case 3:
            printf("\nEnter an updated selling price of %s: \n>>> ", current->data.name);
            scanf("%lf", &current->data.price);
            current->data.profit = current->data.price - current->data.originalPrice;
            break;
        default:
            printf("\nError: Incorrect number entered.");
            break;
    }

    // update the lastUpdated data
    sprintf(current->data.lastUpdated, "%s | %s", getCurrentDate(), getCurrentTime());

    printLinkedlist(*head);
    printf("\n\nUpdated Successfully!");
    printf("\nEnter 'b' to go back.");
}

// for development purposes only
void addTestItem(struct Node **head, struct ProfitPerMonth monthlyProfits[], char name[], int stocks, double price)
{
    struct Item newItem;

    strcpy(newItem.name, name);
    newItem.stocks = stocks;
    newItem.baseStocks = stocks;
    newItem.price = price;
    newItem.originalPrice = price - 5;
    newItem.profit = newItem.price - newItem.originalPrice;

    sprintf(newItem.dateAdded, "%s | %s", getCurrentDate(), getCurrentTime());
    sprintf(newItem.lastUpdated, "%s | %s", getCurrentDate(), getCurrentTime());

    generateId(newItem.id);
    monthlyProfits[getCurrentDateInt()].costs += newItem.stocks * newItem.originalPrice;

    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    newNode->data = newItem;
    newNode->next = NULL;

    if (*head == NULL) {
        *head = newNode;
    } else {
        struct Node *current = *head;
        while(current->next != NULL)
        {
            current = current->next;
        }
        current->next = newNode;
    }
    printf("\nItem added!");
}