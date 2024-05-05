#include "main.h"

void pos_addCartItemHandler(struct Cart *cart, struct Node **head)
{
    char header[] = "Point of Sale";
    char itemId[ID_LENGTH];
    double quantity;

    display_inventoryPromptPage(head, "Enter the item ID:", "Enter 'b' to go back");
    scanf("%s", itemId);

    // get the item with that ID
    struct Node *currentItem = item_getItemById(head, itemId);

    if (strcmp(itemId, "b") == 0 || strcmp(itemId, "B") == 0) return;

    if (currentItem == NULL) {
        display_newUserMessagePage(header, "", "Item not found, please try again.", "", "", "", "");
        sleep(SLEEP_TIME);
        return;
    }

    if (pos_isAlreadyInCart(cart, itemId)) {
        display_newUserMessagePage(header, "", "Item already in the cart.", "", "", "", "");
        sleep(SLEEP_TIME);
        return;
    }

    // ask the user for quantity purchased
    display_newUserMessagePage(header, "", "Enter the quantity:", "", "", "", "");
    scanf("%lf", &quantity);

    // if there's not enough stocks, don't proceed
    if (currentItem->data.stocks < quantity) {
        display_newUserMessagePage(header, "", "Not enough stocks to fulfill the purchase.", "", "", "", "");
        sleep(SLEEP_TIME);
        return;
    }

    pos_addItemToCart(cart, quantity, currentItem->data);

    display_newUserMessagePage(header, "", "Item added to cart!", "", "", "", "");
    sleep(SLEEP_TIME);
}

void pos_deleteCartItemHandler(struct Cart *cart)
{
    char header[] = "Point of Sale";

    if (cart->amountOfItems == 0) {
        display_newUserMessagePage(header, "Enter any key to go back", "No cart item to be deleted", "", "", "", "");
    } else {
        display_pointOfSalePromptPage(cart->items, &cart->amountOfItems, "Enter the number of the item in the cart:", "Enter 'b' to go back");
    }

    char confirmation;
    char userInput[2];
    fflush(stdin);
    scanf("%s", userInput);

    if (strcmp(userInput, "b") == 0 || strcmp(userInput, "B") == 0 || cart->amountOfItems == 0) return;

    // convert user input(string) to int
    int index = atoi(userInput) - 1;

    if (index < 0 || index >= cart->amountOfItems) {
        display_newUserMessagePage(header, "", "Invalid number.", "", "", "", "");
        sleep(SLEEP_TIME);
        return;
    }

    // ask for confirmation to delete
    display_newUserMessagePage("Deleting an Item", "", "Are you sure you want to delete this item?", "[ y / n ]", "", "", "");
    fflush(stdin);
    scanf("%c", &confirmation);

    if (confirmation != 'y' && confirmation != 'Y') return;

    pos_deleteItemFromCart(cart, index);

    display_newUserMessagePage(header, "", "Item deleted from the cart!", "", "", "", "");
    sleep(SLEEP_TIME);
}

void pos_resetCartHandler(struct Cart *cart) 
{
    char action;

    if (cart->amountOfItems == 0) {
        display_newUserMessagePage("Point of Sale", "Enter any key to go back", "No items in the cart", "", "", "", "");
    } else {
        display_newUserMessagePage("Point of Sale", "", "Are you sure you want to remove", "all items in the cart?", "[ y / n ]", "", "");
    }

    fflush(stdin);
    scanf("%c", &action);

    if (action == 'n' || action == 'N' || cart->amountOfItems == 0) return;

    pos_resetCart(cart);

    display_newUserMessagePage("Point of Sale", "", "Cart items removed.", "", "", "", "");
    sleep(SLEEP_TIME);
}

void pos_checkoutHandler(struct Cart *cart, struct Node **head, struct ReportPerMonth monthlyProfits[])
{
    char header[] = "Point of Sale";

    if (cart->amountOfItems == 0) {
        display_newUserMessagePage(header, "Enter any key to go back", "No items in the cart to checkout", "", "", "", "");
    } else {
        display_newUserMessagePage(header, "", "Are you sure you want to checkout", "all items in the cart?", "[ y / n ]", "", "");
    }

    char action;
    fflush(stdin);
    scanf("%c", &action);

    if (action == 'n' || action == 'N' || cart->amountOfItems == 0) return;
    
    utils_generateId(cart->cartId);
    char reciept[MAX_RECEIPT_LENGTH];

    double totalPrice = 0.0;

    // get the total price
    for (int i = 0; i < cart->amountOfItems; i++) {
        int quantity = cart->items[i].quantity;

        struct Node *current = item_getItemById(head, cart->items[i].itemId);

        totalPrice += quantity * current->data.price;
    }

    char totalPriceStr[69];
    char cashier[69];
    double cash;
    sprintf(totalPriceStr, "Total price is: %.2lf", totalPrice);
    display_newUserMessagePage(header, "", totalPriceStr, "Enter the cash:", "", "", "");
    scanf("%lf", &cash);

    // if the cash entered is not enough, don't proceed
    if (cash < totalPrice) {
        display_newUserMessagePage(header, "", "Transaction failed. Not enough cash.", "", "", "", "");
        sleep(SLEEP_TIME);
        return;
    }

    // get the cashier name
    display_newUserMessagePage(header, "", "Enter the name of cashier: (First name only)", "", "", "", "");
    scanf("%s", cashier);

    // TODO: maybe separate this into a function
    // deduct all items from the inventory and tally the revenues
    for (int i = 0; i < cart->amountOfItems; i++) {
        int quantity = cart->items[i].quantity;
        char itemId[ID_LENGTH];
        strcpy(itemId, cart->items[i].itemId);

        // get the item in the inventory
        struct Node *current = item_getItemById(head, itemId);

        // tally the stocks, revenue and profit
        current->data.stocks -= quantity;
        sales_updateRevenue(monthlyProfits, current->data.price, quantity);
        sales_updateProfit(monthlyProfits, current->data.profit, quantity);
        
        // update the data from storage as well
        storage_editItemFromStorageById(itemId, current->data);
        sales_updateReportsFromStorage(monthlyProfits);
    }

    // generate a reciept
    pos_generateReceipt(cart, totalPrice, cash, cashier, reciept);
    
    // save the receipt
    storage_addRecieptToStorage(reciept, cart->cartId);
    pos_saveRecieptMetaData(cart->cartId);

    display_printReceipt(reciept, "", "Enter any key to continue");
    getch();

    // reset the cart
    pos_resetCart(cart);

    display_newUserMessagePage(header, "", "Checkout successfull!", "", "", "", "");
    sleep(SLEEP_TIME);
}

void pos_viewReceiptHandler()
{
    char id[ID_LENGTH];
    char receipt[MAX_RECEIPT_LENGTH];

    display_recieptsPromptPage("Enter the ID of the receipt:", "Enter 'b' to go back");
    fflush(stdin);
    fgets(id, ID_LENGTH, stdin);
    utils_clearNewline(id);
    
    if (strcmp(id, "b") == 0 || strcmp(id, "B") == 0) return;

    int status = storage_getReceiptFromStorageById(id, receipt);

    if (status == 0) {
        display_newUserMessagePage("Viewing a Receipt", "", "Receipt not found.", "", "", "", "");
        sleep(SLEEP_TIME);
        return;
    } 

    display_printReceipt(receipt, "Here is the receipt.", "Enter any key to go back.");
    getch();
}

void pos_deleteReceiptHandler()
{
    char id[ID_LENGTH];
    char receipt[MAX_RECEIPT_LENGTH];
    char confirmation;

    display_recieptsPromptPage("Enter the ID of the receipt you want to delete:", "Enter 'b' to go back");
    fflush(stdin);
    scanf("%s", id);

    if (strcmp(id, "b") == 0 || strcmp(id, "B") == 0) return;
    
    int status = storage_getReceiptFromStorageById(id, receipt);

    if (status == 0) {
        display_newUserMessagePage("Deleting a Receipt", "", "Receipt not found.", "", "", "", "");
        sleep(SLEEP_TIME);
        return;
    } 

    // ask for confirmation to delete
    display_printReceipt(receipt, "Are you sure you want to delete this receipt? [ y / n ]", "This action is irreversable.");
    fflush(stdin);
    scanf("%c", &confirmation);

    if (confirmation != 'y' && confirmation != 'Y') return;

    storage_deleteReceiptFromStorage(id);

    display_newUserMessagePage("Deleting a receipt", "", "Receipt deleted succesfully!", "", "", "", "");
    sleep(SLEEP_TIME);
}

void pos_deleteItemFromCart(struct Cart *cart, int indexToDelete)
{
    // shift the items to the left, overwritting the deleted item
    for (int i = indexToDelete; i < cart->amountOfItems - 1; i++)
        cart->items[i] = cart->items[i + 1];

    // decrease the counter
    cart->amountOfItems -= 1;
}

void pos_resetCart(struct Cart *cart)
{
    // reset the name, itemId, quantity, and price of each item in the cart
    for (int i = 0; i < cart->amountOfItems; i++) {
        strcpy(cart->items[i].name, "");
        strcpy(cart->items[i].itemId, "");
        cart->items[i].quantity = 0;
        cart->items[i].price = 0.0;
        cart->items[i].profit = 0.0;
    }
    // reset the amount of items to zero    
    cart->amountOfItems = 0;
}

void pos_addItemToCart(struct Cart *cart, int quantity, struct Item item)
{
    strcpy(cart->items[cart->amountOfItems].name, item.name);
    strcpy(cart->items[cart->amountOfItems].itemId, item.id);
    cart->items[cart->amountOfItems].quantity = quantity;
    cart->items[cart->amountOfItems].price = item.price;
    cart->items[cart->amountOfItems].profit = item.profit * quantity;

    cart->amountOfItems += 1;
}

bool pos_isAlreadyInCart(struct Cart *cart, char *itemId)
{
    char currentItemId[ID_LENGTH];

    for (int i = 0; i < cart->amountOfItems; i++) {
        strcpy(currentItemId, cart->items[i].itemId);

        if (strcmp(currentItemId, itemId) == 0) return true;
    }

    return false;
}

void pos_generateReceipt(struct Cart *cart, double totalPrice, double cash, char *cashier, char *receiptBuffer) 
{
    // width of the reciept
    int width = 62;
    int receiptTextLength = 60;
    double vatPercentage = 0.12;

    // constructing a centered id
    char idText[receiptTextLength];
    sprintf(idText, "Receipt ID: %s", cart->cartId);
    utils_centerText(width, idText);

    // constructing a centered shop name
    char shopNameText[receiptTextLength];
    strcpy(shopNameText, settings.shopName);
    utils_centerText(width, shopNameText);

    // constructing a centered shop location
    char shopLocationText[receiptTextLength];
    strcpy(shopLocationText, settings.shopLocation);
    utils_centerText(width, shopLocationText);

    // constructing a centered date
    char dateText[receiptTextLength];
    sprintf(dateText, "Date Purchased: %s %s", utils_getCurrentDate(), utils_getCurrentTime());
    utils_centerText(width, dateText);

    char bottomText[receiptTextLength];
    strcpy(bottomText, "THIS IS YOUR OFFICIAL RECEIPT");
    utils_centerText(width, bottomText);

    // constructing a centered thank you
    char tyText[receiptTextLength];
    // strcpy(tyText, "Thank You and Come Again!");
    sprintf(tyText, "Thank You - %s", settings.shopName);
    utils_centerText(width, tyText);

    char blankSpace[receiptTextLength];
    strcpy(blankSpace, " ");
    utils_centerText(width, blankSpace);

    // using snprintf to connect everything into a single string, could've used strcat but someone said this is more efficient because we are manually setting the size of the string each addition
    snprintf(receiptBuffer, MAX_RECEIPT_LENGTH, "%s\n", shopNameText);
    snprintf(receiptBuffer + strlen(receiptBuffer), MAX_RECEIPT_LENGTH - strlen(receiptBuffer), "%s\n", shopLocationText);
    snprintf(receiptBuffer + strlen(receiptBuffer), MAX_RECEIPT_LENGTH - strlen(receiptBuffer), "%s\n", dateText);
    snprintf(receiptBuffer + strlen(receiptBuffer), MAX_RECEIPT_LENGTH - strlen(receiptBuffer), "%s\n", blankSpace);
    snprintf(receiptBuffer + strlen(receiptBuffer), MAX_RECEIPT_LENGTH - strlen(receiptBuffer), "-------------------------------------------------------------\n");
    snprintf(receiptBuffer + strlen(receiptBuffer), MAX_RECEIPT_LENGTH - strlen(receiptBuffer), "%-30s %-10s %-10s %-8s\n", "Item", "Quantity", "Price", "Total");
    snprintf(receiptBuffer + strlen(receiptBuffer), MAX_RECEIPT_LENGTH - strlen(receiptBuffer), "-------------------------------------------------------------\n");

    // each individual item
    for (int i = 0; i < cart->amountOfItems; i++) {
        struct CartItem item = cart->items[i];
        double totalItemPrice = item.quantity * item.price;
        snprintf(receiptBuffer + strlen(receiptBuffer), MAX_RECEIPT_LENGTH - strlen(receiptBuffer), "%-30s %-10d P%-9.2lf P%-7.2lf\n", item.name, item.quantity, item.price, totalItemPrice);
    }

    double VAT = vatPercentage * totalPrice;
    double VATable = totalPrice - VAT;

    // snprintf(receiptBuffer + strlen(receiptBuffer), MAX_RECEIPT_LENGTH - strlen(receiptBuffer), "|------------------------------------------------------------|\n");
    snprintf(receiptBuffer + strlen(receiptBuffer), MAX_RECEIPT_LENGTH - strlen(receiptBuffer), "%s\n", blankSpace);
    snprintf(receiptBuffer + strlen(receiptBuffer), MAX_RECEIPT_LENGTH - strlen(receiptBuffer), "%-52s P%-7.2lf\n", "Total Price", totalPrice);
    snprintf(receiptBuffer + strlen(receiptBuffer), MAX_RECEIPT_LENGTH - strlen(receiptBuffer), "%-52s P%-7.2lf\n", "Cash", cash);
    snprintf(receiptBuffer + strlen(receiptBuffer), MAX_RECEIPT_LENGTH - strlen(receiptBuffer), "%-52s P%-7.2lf\n", "Change", cash - totalPrice);
    snprintf(receiptBuffer + strlen(receiptBuffer), MAX_RECEIPT_LENGTH - strlen(receiptBuffer), "%s\n", blankSpace);
    snprintf(receiptBuffer + strlen(receiptBuffer), MAX_RECEIPT_LENGTH - strlen(receiptBuffer), "%-52s %-8s\n", "Cashier", cashier);
    snprintf(receiptBuffer + strlen(receiptBuffer), MAX_RECEIPT_LENGTH - strlen(receiptBuffer), "%-52s %-8d\n", "No. of Items", cart->amountOfItems);
    snprintf(receiptBuffer + strlen(receiptBuffer), MAX_RECEIPT_LENGTH - strlen(receiptBuffer), "%-52s P%-7.2lf\n", "VATable Sales", VATable);
    snprintf(receiptBuffer + strlen(receiptBuffer), MAX_RECEIPT_LENGTH - strlen(receiptBuffer), "%-52s P%-7.2lf\n", "VAT", VAT);
    snprintf(receiptBuffer + strlen(receiptBuffer), MAX_RECEIPT_LENGTH - strlen(receiptBuffer), "-------------------------------------------------------------\n");

    snprintf(receiptBuffer + strlen(receiptBuffer), MAX_RECEIPT_LENGTH - strlen(receiptBuffer), "%s\n", blankSpace);
    snprintf(receiptBuffer + strlen(receiptBuffer), MAX_RECEIPT_LENGTH - strlen(receiptBuffer), "%s\n", bottomText);
    snprintf(receiptBuffer + strlen(receiptBuffer), MAX_RECEIPT_LENGTH - strlen(receiptBuffer), "%s\n", idText);
    snprintf(receiptBuffer + strlen(receiptBuffer), MAX_RECEIPT_LENGTH - strlen(receiptBuffer), "%s\n", tyText);

}

void pos_saveRecieptMetaData(char *recieptId)
{
    char datePurchased[DATE_LENGTH];
    sprintf(datePurchased, "%s %s", utils_getCurrentDate(), utils_getCurrentTime());

    storage_addRecieptMetaDataToStorage(recieptId, datePurchased);
}
