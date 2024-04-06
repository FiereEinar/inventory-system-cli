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
    struct Node *head = NULL;
    struct ProfitPerMonth monthlyProfits[MONTHS];
    initMonthlyProfits(monthlyProfits);             // fill the monthly values with zero and sets monthly names
    char action;

    // Adding test items for development purposes only
    addItemToList(&head, monthlyProfits, "pencil", 25, 10, 5, 15);
    addItemToList(&head, monthlyProfits, "paper", 10, 55, 44, 15);
    addItemToList(&head, monthlyProfits, "scissors", 15, 56, 47, 15);
    addItemToList(&head, monthlyProfits, "ballpen", 18, 12, 10, 15);
    addItemToList(&head, monthlyProfits, "scraper", 5, 200, 150, 15);
    addItemToList(&head, monthlyProfits, "comb", 4, 46, 40, 15);

    while (true) {
        system("cls");
        menuPage();

        bannerUserInput();
        scanf("%c", &action);

        switch (action)
        {
        case '1':
            viewInventory(&head, monthlyProfits);
            break;
        case '2':
            sellItem(&head, monthlyProfits);
            break;
        case '3':
            system("cls");
            viewReports(monthlyProfits);
            break;
        case 'q':
            return 0;
        }
        // if (strcmp(action, "1") == 0) viewInventory(&head, monthlyProfits);
        // if (strcmp(action, "1") == 0) viewInventory(head);
        // if (strcmp(action, "2") == 0) addItem(&head, monthlyProfits);
        // if (strcmp(action, "3") == 0) deleteItem(&head, monthlyProfits);
        // if (strcmp(action, "4") == 0) editItem(&head);
        // if (strcmp(action, "5") == 0) restockItem(&head, monthlyProfits);
        // if (strcmp(action, "6") == 0) searchItem(&head);
        // if (strcmp(action, "7") == 0) sellItem(&head, monthlyProfits);
        // if (strcmp(action, "8") == 0) viewReports(monthlyProfits);
        // if (strcmp(action, "b") == 0) printActions();
    }
    // TODO: maybe clear the memory of head? there's an available function for that
}

void viewInventory(struct Node **head, struct ProfitPerMonth monthlyProfits[])
{
    char action;
    system("cls");

    inventoryPage(head);

    while (true) {
        bannerUserInput();
        fflush(stdin);
        scanf("%c", &action);

        switch (action)
        {
        case '1':
            addItem(head, monthlyProfits);
            system("cls");
            inventoryPage(head);
            break;
        case '2':
            deleteItem(head, monthlyProfits);
            system("cls");
            inventoryPage(head);
            break;
        case '3':
            editItem(head);
            system("cls");
            inventoryPage(head);
            break;
        case '4':
            restockItem(head, monthlyProfits);
            system("cls");
            inventoryPage(head);
            break;
        case '5':
            searchItem(head);
            break;
        case '6':
            viewItemDetails(head);
            break;
        case 'b':
            return;
        }
    }
}

void viewItemDetails(struct Node **head)
{
    char itemId[ID_LENGTH];

    printf("Enter the item ID:");
    bannerUserInput();
    scanf("%s", itemId);

    struct Node *itemData = getItemById(head, itemId);

    if (itemData == NULL) {
        printf("\nItem not found, please try again.");
        sleep(SLEEP_TIME);
        system("cls");
        inventoryPage(head);
        return;
    }
    
    printItemData(itemData->data);
}

void printItemData(struct Item item)
{
    system("cls");
    printf("\nMore details about the item: \n");
    printf("\nItem Name:\t\t\t\t%s", item.name);
    printf("\nStocks:\t\t\t\t\t%d / %d", item.stocks, item.baseStocks);
    printf("\nPrice:\t\t\t\t\t%.2lf", item.price);
    printf("\nOriginal Price:\t\t\t\t%.2lf", item.originalPrice);
    printf("\nProfit per item:\t\t\t%.2lf", item.profit);
    printf("\nDate Added:\t\t\t\t%s", item.dateAdded);
    printf("\nLast Updated:\t\t\t\t%s", item.lastUpdated);
    printf("\nItem ID:\t\t\t\t%s", item.id);

    printf("\n\nEnter 'b' to go back.");
}

void viewReports(struct ProfitPerMonth monthlyProfits[])
{
    printf("Month:\t\t\t\tCosts:\t\tRevenue:\t\tProfit:\n");
    for (int i = 0; i < MONTHS; i++)
    {
        printf
        (
            "%d. %-15s\t\tP%-10.2lf\tP%-10.2lf\t\tP%-10.2lf\n", 
            i + 1, monthlyProfits[i].month, monthlyProfits[i].costs, monthlyProfits[i].revenue, monthlyProfits[i].profit
        );
    }
    printf("\nTotal Costs:\t\t\t\t\t\t\t\tP%.2lf", getTotalCosts(monthlyProfits));
    printf("\nTotal Revenue:\t\t\t\t\t\t\t\tP%.2lf", getTotalRevenue(monthlyProfits));
    printf("\nTotal Profit:\t\t\t\t\t\t\t\tP%.2lf", getTotalProfit(monthlyProfits));

    char back;
    printf("\n\nEnter 'b' to go back.");
    bannerUserInput();
    fflush(stdin);
    scanf("%c", &back);
}

double getTotalProfit(struct ProfitPerMonth monthlyProfits[])
{
    double total = 0;

    for (int i = 0; i < MONTHS; i++)
        total += monthlyProfits[i].profit;

    return total;
}

double getTotalCosts(struct ProfitPerMonth monthlyProfits[])
{
    double total = 0;

    for (int i = 0; i < MONTHS; i++)
        total += monthlyProfits[i].costs;

    return total;
}

double getTotalRevenue(struct ProfitPerMonth monthlyProfits[])
{
    double total = 0;

    for (int i = 0; i < MONTHS; i++)
        total += monthlyProfits[i].revenue;

    return total;
}

void initMonthlyProfits(struct ProfitPerMonth monthlyProfits[])
{
    char months[MONTHS][15] = 
    {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };

    // initialize the monthly data
    for (int i = 0; i < MONTHS; ++i) 
    {
        strcpy(monthlyProfits[i].month, months[i]);
        monthlyProfits[i].costs = 0.0;
        monthlyProfits[i].revenue = 0.0;
        monthlyProfits[i].profit = 0.0;
    }
}

void printActions()
{
    system("cls");
    topBar("Inventory Management System");
    printf("\nEnter the action you want to take:\n");

    printf("\n1. view all items in the inventory.");

    printf("\n2. add an item in the inventory.");
    printf("\n3. delete an item in the inventory.");
    printf("\n4. edit an item in the inventory.");
    printf("\n5. restock an item in the inventory.");
    printf("\n6. search an item in the inventory.");
    
    printf("\n7. sell an item.");
    printf("\n8. view sales report.");

    printf("\n\nEnter 'q' to exit.");
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

// print the linked list values
void printLinkedlist(const struct Node *head)
{
    int i = 1;
    printf("\nItem:\t\t\tStocks:\t Selling Price:\t Profit/item:\t Id:");
    // loop over the entire list and print the data on each iteration
    while (head != NULL)
    {
        // TODO: truncate the name if it's too long
        // if (strlen(head->data.name) >= 8) { / ... / }
        printf
        (
            "\n%d. %-20s\t%d/%d\t P%-14.2lf %.2lf\t\t %s",
            i, head->data.name, head->data.stocks, head->data.baseStocks, head->data.price, head->data.profit, head->data.id
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
