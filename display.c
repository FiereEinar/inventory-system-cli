#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

void itemDataPage(struct Item item)
{
    topBar("Details of an item");
    bannerBlankBorder();

    printf("::  Item Name:\t\t\t\t\t\t\t%s\t\t\t\t\t\t\t\t\t  ::\n", item.name);
    printf("::  Stocks:\t\t\t\t\t\t\t%d/%d\t\t\t\t\t\t\t\t\t  ::\n", item.stocks, item.baseStocks);
    printf("::  Price:\t\t\t\t\t\t\t%.2lf\t\t\t\t\t\t\t\t\t  ::\n", item.price);
    printf("::  Original Price:\t\t\t\t\t\t%.2lf\t\t\t\t\t\t\t\t\t  ::\n", item.originalPrice);
    printf("::  Profit per item:\t\t\t\t\t\t%.2lf\t\t\t\t\t\t\t\t\t  ::\n", item.profit);
    printf("::  Date Added:\t\t\t\t\t\t\t%s\t\t\t\t\t\t\t  ::\n", item.dateAdded);
    printf("::  Last Updated:\t\t\t\t\t\t%s\t\t\t\t\t\t\t  ::\n", item.lastUpdated);
    printf("::  Item ID:\t\t\t\t\t\t\t%s\t\t\t\t\t\t\t\t  ::\n", item.id);

    printMinimumScreenHeight(3);
    bannerBlankBorderTextCen("Enter 'b' to go back Home Page.");
    bannerFullBorder();
}

void newUserMessagePage(char *header, char *bottomMessage, char *message1, char *message2, char *message3, char *message4, char *message5)
{
    system("cls");
    userMessagePage(header, bottomMessage, message1, message2, message3, message4, message5);
    bannerUserInput();
}

void userMessagePage(char *header, char *bottomMessage, char *message1, char *message2, char *message3, char *message4, char *message5)
{
    topBar(header);

    for (int i = 0; i < 9; i++) bannerBlankBorder();

    bannerBlankBorderTextCen(message1);
    bannerBlankBorderTextCen(message2);
    bannerBlankBorderTextCen(message3);
    bannerBlankBorderTextCen(message4);
    bannerBlankBorderTextCen(message5);

    for (int i = 0; i < 9; i++) bannerBlankBorder();

    bannerBlankBorderTextCen(bottomMessage);
    bannerFullBorder();
}

void printMinimumScreenHeight(int itemsLength)
{
    int min = 20;

    if (itemsLength >= min) return;
    for (int i = 0; i < min - itemsLength; i++)
    {
        bannerBlankBorder();
    }
}

void salesReportPage(struct ProfitPerMonth monthlyProfits[])
{
    bannerFullBorder();
    bannerBlankBorder();
    printReportHeader();
    bannerFullBorderSection();

    printMonthlyReports(monthlyProfits);
    printMinimumScreenHeight(MONTHS - 1);

    bannerBlankBorderTextCen("'b' to go back to Menu Page");
    bannerFullBorder();
}

void printReportHeader()
{
    printf("::  Month\t\t\t\t\t\t\t\tCosts\t\t\tRevenue\t\t\tProfit\t\t  ::\n");
}

void printMonthlyReports(struct ProfitPerMonth monthlyProfits[])
{
    for (int i = 0; i < MONTHS; i++)
    {
        printf
        (
            "::  %d. %-15s\t\t\t\t\t\t\tP%-10.2lf\t\tP%-10.2lf\t\tP%-10.2lf\t  ::\n", 
            i + 1, monthlyProfits[i].month, monthlyProfits[i].costs, monthlyProfits[i].revenue, monthlyProfits[i].profit
        );
    }
    bannerFullBorderSection();
    printf
    (
        "::  Total\t\t\t\t\t\t\t\tP%-10.2lf\t\tP%-10.2lf\t\tP%-10.2lf\t  ::\n", 
        getTotalCosts(monthlyProfits), getTotalRevenue(monthlyProfits), getTotalProfit(monthlyProfits)
    );
}

void inventoryPage(struct Node **head)
{
    bannerFullBorder();
    bannerBlankBorder();
    printItemHeader();
    bannerFullBorderSection();
    bannerBlankBorder();
    printItemList(head);

    printMinimumScreenHeight(getListSize(head) - 3);
    bannerBlankBorder();

    bannerBlankBorderTextCen("1. Add | 2. Delete | 3. Edit | 4. Restock | 5. Search | 6. Item Details");
    bannerBlankBorderTextCen("'b' to go back to Menu Page");

    bannerFullBorder();
}

void printItemHeader()
{
    printf("::  Item\t\t\t\t\t\t\tStocks\t\tSelling Price\t\tProfit/item\t\tId\t  ::\n");
}

void printItemList(struct Node **head)
{
    struct Node *current = *head;
    int i = 1;
    // loop over the entire list and print the data on each iteration
    while (current != NULL)
    {
        // TODO: truncate the name if it's too long
        // if (strlen(current->data.name) >= 8) { / ... / }
        printf
        (
            "::  %d. %-57s%d/%-12d\tP%-14.2lf\t\t%.2lf\t\t\t%s  ::\n",
            i, current->data.name, current->data.stocks, current->data.baseStocks, current->data.price, current->data.profit, current->data.id
        );
        // iterator
        current = current->next;
        i++;
    }
}

void menuPage()
{
    topBar("Inventory Management System");

    bannerBlankBorder();
    bannerBlankBorder();
    bannerBlankBorder();
    bannerBlankBorder();
    bannerBlankBorder();
    bannerBlankBorder();
    bannerBlankBorder();

    bannerBlankBorderTextCen("Select a page by number");

    bannerBlankBorder();

    bannerBlankBorderTextCen("1. Inventory Page");
    bannerBlankBorderTextCen("2. Point of Sale Page");
    bannerBlankBorderTextCen("3. Sales Reports");

    bannerBlankBorder();
    bannerBlankBorder();
    bannerBlankBorder();
    bannerBlankBorder();
    bannerBlankBorder();
    bannerBlankBorder();
    bannerBlankBorder();
    bannerBlankBorder();
    bannerBlankBorder();
    bannerBlankBorder();

    bannerBlankBorderTextCen("Type 'q' to quit.");
    bannerFullBorder();
}

void topBar(char *text) {
    bannerFullBorder();

    bannerBlankBorder();
    bannerBlankBorderTextCen(text);
    bannerBlankBorder();

    bannerFullBorder();
}

void bannerFullBorder () {
    for ( int i = 0; i < 14; i++ )
        printf ("::::::::::");
    printf ("\n");
}

void bannerBlankBorderTextCen (char *text) {
    // calculate the padding needed to center
    int length = strlen (text) - 1;  // Remove the terminal '\0'
    int pad = (length >= 134) ? 0 : ((134 - length) / 2);

    printf (":: %*s%s%*s ::\n", pad, " ", text, 133 - pad - length, " ");
}

void bannerBlankBorderText (char *text) {
    printf (":: %134s ::\n", text);
}

void bannerBlankBorderTextLeft (char *text) {
    printf (":: %-134s ::\n", text);
}

void bannerBlankBorder () {
    printf (":: %134s ::\n", "");
}

void bannerUserInput () {
    printf ("\n>>> ");
}

void bannerFullBorderSection () {
    printf (":: ______________________________________________________________________________________________________________________________________ ::\n");
}