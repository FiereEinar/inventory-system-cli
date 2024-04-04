#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

#include "main.h"

int main()
{
    srand(time(NULL));
    // the head is initially set to NULL
    struct Node *head = NULL;
    // action is the placeholder for determining the action that the user wants to take
    char action[5];
    printf("\nType 'help' to see all available commands.");

    printf("\nAdding test items for development purposes only.");
    addTestItem(&head, "pencil", 25, 10);
    addTestItem(&head, "paper", 50, 1140);
    addTestItem(&head, "scissors", 15, 56);
    addTestItem(&head, "ballpen", 18, 120);
    addTestItem(&head, "scraper", 46, 912);
    addTestItem(&head, "comb", 4, 46);

    printActions();
    do {
        printf("\n\n>>> ");
        scanf("%s", action);

        //if (strcmp(action, "1") == 0) printHelp();
        if (strcmp(action, "1") == 0) printLinkedlist(head);
        if (strcmp(action, "2") == 0) addItem(&head);
        if (strcmp(action, "3") == 0) deleteItem(&head);
        if (strcmp(action, "4") == 0) updateItem(&head);
        if (strcmp(action, "5") == 0) searchItem(&head);
        if (strcmp(action, "6") == 0) sellItem(&head);
        if (strcmp(action, "b") == 0) printActions();
    } while (strcmp(action, "q") != 0);
    // TODO: maybe clear the memory of head? there's an available function for that
    return 0;
}

void printActions()
{
    system("cls");
    printf("\nWhat do you want to do?\n");

//    printf("\n1. print all available commands.");
//    printf("\n2. exit out the operation.");
    printf("\n1. view all items in the inventory.");
    printf("\n2. add an item to inventory.");
    printf("\n3. delete an item to inventory.");
    printf("\n4. update an item to inventory.");
    printf("\n5. search an item to inventory.");
    printf("\n6. sell an item.");

    printf("\n\nEnter 'q' to exit.");
    printf("\nEnter 'b' to go back.");
}

int getListSize(const struct Node *head)
{
    int size = 0;
    // traverse the list and increment size on each iteration, then return it
    while (head != NULL)
    {
        head = head->next;
        size++;
    }
    return size;
}

void printHelp()
{
    system("cls");
    printf("\nAvailable Commands: ");
    printf("\n1. help \t print all available commands.");
    printf("\n2. exit \t exit out the operation.");
    printf("\n3. view \t print all items in the inventory.");
    printf("\n4. add \t\t add an item to inventory.");
    printf("\n5. delete \t delete an item to inventory.");
    printf("\n6. update \t update an item to inventory.");
    printf("\n7. search\t search an item to inventory.");
    printf("\n8. sale\t\t point of sale.");
}

// print the linked list values
void printLinkedlist(const struct Node *head)
{
    int i = 1;
    printf("\nItem:\t\t\tStocks:\t Price:\t\t Date Added:\t\t Last Updated:\t\t Id:");
    // loop over the entire list and print the data on each iteration
    while (head != NULL)
    {
        // TODO: truncate the name if it's too long
        // if (strlen(head->data.name) >= 8) printf("\n");
        printf
        (
            "\n%d. %-20s\t%d/%d\t P%-14.2lf %s\t %s\t %s",
            i, head->data.name, head->data.stocks, head->data.baseStocks, head->data.price, head->data.dateAdded, head->data.lastUpdated, head->data.id
        );
        // iterator
        head = head->next;
        i++;
    }
}

void freeLinkedList(struct Node **head)
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
