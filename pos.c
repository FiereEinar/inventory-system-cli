#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <conio.h>
#include <string.h>
#include <stdbool.h>

#include "main.h"

// handles the process of selling an item
void sellItemHandler(struct Node **head, struct ReportPerMonth monthlyProfits[])
{
    newUserMessagePage("Point of Sale", "Enter 'b' to go back", "Enter the item ID: ", "", "", "", "");

    char itemId[ID_LENGTH];
    double quantity;

    scanf("%s", itemId);

    // get the item with that ID
    struct Node *current = getItemById(head, itemId);

    if (strcmp(itemId, "b") == 0 || strcmp(itemId, "B") == 0) return;

    if (current == NULL) {
        newUserMessagePage("Point of Sale", "", "Item not found, please try again.", "", "", "", "");
        sleep(SLEEP_TIME);
        return;
    }

    // ask the user for quantity purchased
    newUserMessagePage("Point of Sale", "", "Enter the quantity purchased: ", "", "", "", "");
    scanf("%lf", &quantity);

    // if there's not enough stocks, don't proceed
    if (current->data.stocks < quantity) {
        newUserMessagePage("Point of Sale", "", "Not enough stocks to fulfill the purchase.", "", "", "", "");
        sleep(SLEEP_TIME);
        return;
    }

    // once we get the item, update the stocks and tally the revenue
    current->data.stocks -= (int)quantity;
    updateRevenue(monthlyProfits, current->data.price, quantity);
    updateProfit(monthlyProfits, current->data.profit, quantity);
    
    // update the data from storage as well
    editItemFromStorageById(itemId, current->data);
    updateReportsFromStorage(monthlyProfits);

    newUserMessagePage("Point of Sale", "", "Item purchased successfully", "", "", "", "");
    sleep(SLEEP_TIME);
}

// handles the event cycle in point of sale page
void viewPos(struct Cart *cart, struct Node **head, struct ReportPerMonth monthlyProfits[])
{
    char action;

    while (true){
        system("cls");
        pointOfSalePage(cart->items, &cart->amountOfItems);
        bannerUserInput();
        scanf("%c", &action);

        switch (action)
        {
        case '1':
            addCartItemHandler(cart, head);
            break;
        case '2':
            deleteCartItemHandler(cart);
            break;
        case '3':
            checkoutHandler(cart, head, monthlyProfits);
            break;
        case '4':
            resetCartHandler(cart);
            break;
        case 'b':
            return;
        }
    }
}

void addCartItemHandler(struct Cart *cart, struct Node **head)
{
    char header[] = "Point of Sale";
    newUserMessagePage(header, "Enter 'b' to go back", "Enter the item ID:", "", "", "", "");

    char itemId[ID_LENGTH];
    double quantity;

    scanf("%s", itemId);

    // get the item with that ID
    struct Node *current = getItemById(head, itemId);

    if (strcmp(itemId, "b") == 0 || strcmp(itemId, "B") == 0) return;

    if (current == NULL) {
        newUserMessagePage(header, "", "Item not found, please try again.", "", "", "", "");
        sleep(SLEEP_TIME);
        return;
    }

    // ask the user for quantity purchased
    newUserMessagePage(header, "", "Enter the quantity:", "", "", "", "");
    scanf("%lf", &quantity);

    // if there's not enough stocks, don't proceed
    if (current->data.stocks < quantity) {
        newUserMessagePage(header, "", "Not enough stocks to fulfill the purchase.", "", "", "", "");
        sleep(SLEEP_TIME);
        return;
    }

    addItemToCart(cart, quantity, current->data);

    newUserMessagePage(header, "", "Item added to cart!", "", "", "", "");
    sleep(SLEEP_TIME);
}

void addItemToCart(struct Cart *cart, int quantity, struct Item item)
{
    strcpy(cart->items[cart->amountOfItems].name, item.name);
    strcpy(cart->items[cart->amountOfItems].itemId, item.id);
    cart->items[cart->amountOfItems].quantity = quantity;
    cart->items[cart->amountOfItems].price = item.price;

    cart->amountOfItems += 1;
}


void deleteCartItemHandler(struct Cart *cart)
{
    char header[] = "Point of Sale";

    if (cart->amountOfItems == 0) {
        newUserMessagePage(header, "Enter any key to go back", "No cart item to be deleted", "", "", "", "");
    } else {
        newUserMessagePage(header, "Enter 'b' to go back", "Enter the number of the item:", "", "", "", "");
    }

    char userInput[2];
    fflush(stdin);
    scanf("%s", userInput);

    if (strcmp(userInput, "b") == 0 || strcmp(userInput, "B") == 0 || cart->amountOfItems == 0) return;

    // convert user input(string) to int
    int index = atoi(userInput) - 1;

    if (index < 0 || index >= cart->amountOfItems) {
        newUserMessagePage(header, "", "Invalid number.", "", "", "", "");
        sleep(SLEEP_TIME);
        return;
    }

    // shift the items to the left, overwritting the deleted item
    for (int i = index; i < cart->amountOfItems - 1; i++)
        cart->items[i] = cart->items[i + 1];

    // decrease the counter
    cart->amountOfItems -= 1;

    newUserMessagePage(header, "", "Item deleted from the cart!", "", "", "", "");
    sleep(SLEEP_TIME);
}



void resetCartHandler(struct Cart *cart) 
{
    char action;

    newUserMessagePage("Point of Sale", "", "Are you sure you want to remove", "all items in the cart?", "[ y / n ]", "", "");
    fflush(stdin);
    scanf("%c", &action);

    if (action == 'n' || action == 'N') return;

    resetCart(cart);

    newUserMessagePage("Point of Sale", "", "Cart items removed.", "", "", "", "");
    sleep(SLEEP_TIME);
}

void resetCart(struct Cart *cart)
{
    // reset the name, itemId, quantity, and price of each item in the cart
    for (int i = 0; i < cart->amountOfItems; i++) {
        strcpy(cart->items[i].name, "");
        strcpy(cart->items[i].itemId, "");
        cart->items[i].quantity = 0;
        cart->items[i].price = 0.0;
    }
    // reset the amount of items to zero
    cart->amountOfItems = 0;
}

void checkoutHandler(struct Cart *cart, struct Node **head, struct ReportPerMonth monthlyProfits[])
{
    char header[] = "Point of Sale";

    if (cart->amountOfItems == 0) {
        newUserMessagePage(header, "Enter any key to go back", "No items in the cart to checkout", "", "", "", "");
    } else {
        newUserMessagePage("Point of Sale", "", "Are you sure you want to checkout", "all items in the cart?", "[ y / n ]", "", "");
    }

    char action;
    fflush(stdin);
    scanf("%c", &action);

    if (action == 'n' || action == 'N' || cart->amountOfItems == 0) return;
    
    generateId(cart->cartId);
    char reciept[MAX_RECEIPT_LENGTH];

    // TODO: maybe separate this into a function
    // deduct all items from the inventory and tally the revenues
    for (int i = 0; i < cart->amountOfItems; i++) {
        int quantity = cart->items[i].quantity;
        char itemId[ID_LENGTH];
        strcpy(itemId, cart->items[i].itemId);

        // get the item in the inventory
        struct Node *current = getItemById(head, itemId);

        // tally the stocks, revenue and profit
        current->data.stocks -= quantity;
        updateRevenue(monthlyProfits, current->data.price, quantity);
        updateProfit(monthlyProfits, current->data.profit, quantity);
        
        // update the data from storage as well
        editItemFromStorageById(itemId, current->data);
        updateReportsFromStorage(monthlyProfits);
    }

    // generate a reciept
    system("cls");
    generateReceipt(cart, reciept);
    addRecieptToStorage(reciept, cart->cartId);
    printf("%s", reciept);

    printf("\nEnter any key to continue");
    bannerUserInput();
    getch();
    // reset the cart
    resetCart(cart);

    newUserMessagePage(header, "", "Checkout successfull!", "", "", "", "");
    sleep(SLEEP_TIME);
}

void generateReceipt(struct Cart *cart, char *receiptBuffer) 
{
    // width of the reciept
    int width = 62;

    // constructing a centered id
    char idText[100];
    sprintf(idText, "Receipt ID: %s", cart->cartId);
    centerText(width, idText);

    // constructing a centered date
    char dateText[100];
    sprintf(dateText, "Date Purchased: %s %s", getCurrentDate(), getCurrentTime());
    centerText(width, dateText);

    // constructing a centered thank you
    char tyText[100] = "Thank You and Come Again!";
    centerText(width, tyText);

    // using snprintf to connect everything into a single string
    // snprintf(receiptBuffer, MAX_RECEIPT_LENGTH, "Receipt for Cart ID: %s\n", cart->cartId);
    // snprintf(receiptBuffer, MAX_RECEIPT_LENGTH, "|%s|\n", idText);
    // snprintf(receiptBuffer + strlen(receiptBuffer), MAX_RECEIPT_LENGTH - strlen(receiptBuffer), "Date Purchased: %s %s\n\n", getCurrentDate(), getCurrentTime());
    snprintf(receiptBuffer + strlen(receiptBuffer), MAX_RECEIPT_LENGTH - strlen(receiptBuffer), "_______________________________________________________________\n");
    snprintf(receiptBuffer + strlen(receiptBuffer), MAX_RECEIPT_LENGTH - strlen(receiptBuffer), "|%s|\n", idText);
    snprintf(receiptBuffer + strlen(receiptBuffer), MAX_RECEIPT_LENGTH - strlen(receiptBuffer), "|%s|\n", dateText);
    snprintf(receiptBuffer + strlen(receiptBuffer), MAX_RECEIPT_LENGTH - strlen(receiptBuffer), "|-------------------------------------------------------------|\n");
    snprintf(receiptBuffer + strlen(receiptBuffer), MAX_RECEIPT_LENGTH - strlen(receiptBuffer), "|%-30s %-10s %-10s %-8s|\n", "Item", "Quantity", "Price", "Total");
    snprintf(receiptBuffer + strlen(receiptBuffer), MAX_RECEIPT_LENGTH - strlen(receiptBuffer), "|-------------------------------------------------------------|\n");

    double totalPrice = 0.0;

    for (int i = 0; i < cart->amountOfItems; i++) {
        struct CartItem item = cart->items[i];
        double totalItemPrice = item.quantity * item.price;
        totalPrice += totalItemPrice;
        snprintf(receiptBuffer + strlen(receiptBuffer), MAX_RECEIPT_LENGTH - strlen(receiptBuffer), "|%-30s %-10d $%-9.2f $%-7.2f|\n", item.name, item.quantity, item.price, totalItemPrice);
    }

    snprintf(receiptBuffer + strlen(receiptBuffer), MAX_RECEIPT_LENGTH - strlen(receiptBuffer), "|-------------------------------------------------------------|\n");
    snprintf(receiptBuffer + strlen(receiptBuffer), MAX_RECEIPT_LENGTH - strlen(receiptBuffer), "|%-52s $%-7.2f|\n", "Total Price:", totalPrice);
    snprintf(receiptBuffer + strlen(receiptBuffer), MAX_RECEIPT_LENGTH - strlen(receiptBuffer), "|%s|\n", tyText);
    snprintf(receiptBuffer + strlen(receiptBuffer), MAX_RECEIPT_LENGTH - strlen(receiptBuffer), "|_____________________________________________________________|\n");

}
