#include <stdio.h>
#include <string.h>
#include "main.h"

void inventoryPage(struct Node **head)
{
    bannerFullBorder();
    bannerBlankBorder();
    printItemHeader();
    bannerFullBorderSection();
    bannerBlankBorder();
    printItemList(head);
    bannerBlankBorder();

    bannerBlankBorderTextCen("1. Add | 2. Delete | 3. Edit | 4. Restock");
    bannerBlankBorderTextCen("5. Search | 6. Item Details");
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