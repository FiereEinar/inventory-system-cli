#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

// the layout breaks if the terminal is not full screen
void askUserToFullScreen()
{
    char x;
    system("cls");
    printf("\n\n\n\n");
    printf("::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
    printf("::                                                                          ::\n");
    printf("::                                                                          ::\n");
    printf("::              Please ensure that the window is at full screen             ::\n");
    printf("::                                                                          ::\n");
    printf("::                        Enter any key to continue                         ::\n");
    printf("::                                                                          ::\n");
    printf("::                                                                          ::\n");
    printf("::                                                                          ::\n");
    printf("::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
    bannerUserInput();
    scanf("%c", &x);
}

void itemDataPage(struct Item item)
{
    topBar("Details of an item");
    bannerBlankBorder();

    printf("::  Item Name:\t\t\t\t\t\t\t%-20s\t\t\t\t\t\t\t  ::\n", item.name);
    printf("::  Stocks:\t\t\t\t\t\t\t%d/%-20d\t\t\t\t\t\t\t  ::\n", item.stocks, item.baseStocks);
    printf("::  Selling Price:\t\t\t\t\t\t%-20.2lf\t\t\t\t\t\t\t  ::\n", item.price);
    printf("::  Original Price:\t\t\t\t\t\t%-20.2lf\t\t\t\t\t\t\t  ::\n", item.originalPrice);
    printf("::  Profit per item:\t\t\t\t\t\t%-20.2lf\t\t\t\t\t\t\t  ::\n", item.profit);
    printf("::  Date Added:\t\t\t\t\t\t\t%-15s\t\t\t\t\t\t\t  ::\n", item.dateAdded);
    printf("::  Last Updated:\t\t\t\t\t\t%-15s\t\t\t\t\t\t\t  ::\n", item.lastUpdated);
    printf("::  Item ID:\t\t\t\t\t\t\t%-15s\t\t\t\t\t\t\t\t  ::\n", item.id);

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

void salesReportPage(struct ReportPerMonth monthlyProfits[])
{
    bannerFullBorder();
    bannerBlankBorder();
    printReportHeader();
    bannerFullBorderSection();

    printMonthlyReports(monthlyProfits);
    printMinimumScreenHeight(MONTHS - 2);

    bannerBlankBorderTextCen("Enter the number of month to see more details.");
    bannerBlankBorderTextCen("'b' to go back to Menu Page");
    bannerFullBorder();
}

void salesPerDayReportPage(struct ReportPerDay day[], struct ReportPerMonth monthlyProfits[], int month)
{
    bannerFullBorder();
    bannerBlankBorder();
    printMonthlyReportHeader(monthlyProfits[month].month);
    bannerFullBorderSection();

    printPerDayReports(day, monthlyProfits, month);
    printMinimumScreenHeight(DAYS_IN_MONTH - 1);

    bannerBlankBorder();
    bannerBlankBorderTextCen("'b' to go back");
    bannerFullBorder();
}

void printPerDayReports(struct ReportPerDay day[], struct ReportPerMonth monthlyProfits[], int month)
{
    for (int i = 0; i < DAYS_IN_MONTH; i++)
    {
        // day[i].profit = getProfitForDay(day[i]);
        printf
        (
            "::  %-4d  \t\t\t\t\tP%-10.2lf\t\tP%-10.2lf\t\tP%-10.2lf\t\tP%-10.2lf\t  ::\n", 
            i + 1, day[i].costs, day[i].additionalCosts, day[i].revenue, getProfitForDay(day[i])
        );
    }
    bannerFullBorderSection();
    printf
    (
        "::  Total\t\t\t\t\tP%-10.2lf\t\tP%-10.2lf\t\tP%-10.2lf\t\tP%-10.2lf\t  ::\n", 
        monthlyProfits[month].costs, monthlyProfits[month].additionalCosts, monthlyProfits[month].revenue, getProfitForMonth(monthlyProfits[month])
    );
}

void printMonthlyReportHeader(char *month)
{
    printf("::  %s\t\t\t\t\tCosts\t\t\tExtra Costs\t\tRevenue\t\t\tProfit\t\t  ::\n", month);
}

void printReportHeader()
{
    printf("::  Month\t\t\t\t\tCosts\t\t\tExtra Costs\t\tRevenue\t\t\tProfit\t\t  ::\n");
}

void printMonthlyReports(struct ReportPerMonth monthlyProfits[])
{
    for (int i = 0; i < MONTHS; i++)
    {
        // monthlyProfits[i].profit = getProfitForMonth(monthlyProfits[i]);
        // monthlyProfits[i].profit = monthlyProfits[i].revenue - monthlyProfits[i].costs - monthlyProfits[i].additionalCosts;
        printf
        (
            "::  %d. %-15s\t\t\t\tP%-10.2lf\t\tP%-10.2lf\t\tP%-10.2lf\t\tP%-10.2lf\t  ::\n", 
            i + 1, monthlyProfits[i].month, monthlyProfits[i].costs, monthlyProfits[i].additionalCosts, monthlyProfits[i].revenue, getProfitForMonth(monthlyProfits[i])
        );
    }
    bannerFullBorderSection();
    printf
    (
        "::  Total\t\t\t\t\tP%-10.2lf\t\tP%-10.2lf\t\tP%-10.2lf\t\tP%-10.2lf\t  ::\n", 
        getTotalCosts(monthlyProfits), getTotalExtraCosts(monthlyProfits), getTotalRevenue(monthlyProfits), getTotalProfit(monthlyProfits)
    );
}

void inventoryPage(struct Node **head)
{
    bannerFullBorder();
    bannerBlankBorder();
    printItemHeader();
    bannerFullBorder();
    // bannerFullBorderSection();
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
    printf("::\tItem\t\t\t\t\t\tStocks\t\tSelling Price\t\tProfit/item\t\tId\t\t  ::\n");
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
            "::  %3d. %-45s\t%d/%-12d\tP%-14.2lf\t\t%.2lf\t\t\t%s\t  ::\n",
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
