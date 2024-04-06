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
    char action[5];                                 // action of user, string because there are 'b' or 'q' options

    // Adding test items for development purposes only
    addTestItem(&head, monthlyProfits, "pencil", 25, 10);
    // addTestItem(&head, monthlyProfits, "paper", 10, 55);
    // addTestItem(&head, monthlyProfits, "scissors", 15, 56);
    // addTestItem(&head, monthlyProfits, "ballpen", 18, 12);
    // addTestItem(&head, monthlyProfits, "scraper", 5, 200);
    // addTestItem(&head, monthlyProfits, "comb", 4, 46);

    printActions();
    do {
        printf("\n\n>>> ");
        scanf("%s", action);

        if (strcmp(action, "1") == 0) viewInventory(head);
        if (strcmp(action, "2") == 0) addItem(&head, monthlyProfits);
        if (strcmp(action, "3") == 0) deleteItem(&head, monthlyProfits);
        if (strcmp(action, "4") == 0) editItem(&head);
        if (strcmp(action, "5") == 0) restockItem(&head, monthlyProfits);
        if (strcmp(action, "6") == 0) searchItem(&head);
        if (strcmp(action, "7") == 0) sellItem(&head, monthlyProfits);      // TODO: restrict selling if quantity is greater than stocks
        if (strcmp(action, "8") == 0) viewReports(monthlyProfits);

        if (strcmp(action, "b") == 0) printActions();
    } while (strcmp(action, "q") != 0);
    // TODO: maybe clear the memory of head? there's an available function for that
    return 0;
}

void viewInventory(const struct Node *head)
{
    printLinkedlist(head);
    printf("\n\nEnter 'b' to go back.");
}

void viewReports(struct ProfitPerMonth monthlyProfits[])
{
    printf("Month:\t\t\tCost:\t\tRevenue:\t\tProfit:\n");
    for (int i = 0; i < MONTHS; i++)
    {
        printf
        (
            "%-15s\t\tP%-10.2lf\tP%-10.2lf\t\tP%-10.2lf\n", 
            monthlyProfits[i].month, monthlyProfits[i].costs, monthlyProfits[i].revenue, monthlyProfits[i].profit
        );
    }
    printf("\nTotal Costs:\t\t\t\t\t\t\tP%.2lf", getTotalCosts(monthlyProfits));
    printf("\nTotal Profit:\t\t\t\t\t\t\tP%.2lf", getTotalProfit(monthlyProfits));

    printf("\n\nEnter 'q' to exit.");
    printf("\nEnter 'b' to go back.");
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
