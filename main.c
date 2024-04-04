#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

#define NAME_SIZE 20
#define ID_LENGTH 10

// I'm using linked list because it makes that list management easier.
// adding, deleting, updating, etc. is so much easier than using traditional arrays

// structure of an individual item
struct Item
{
    char name[NAME_SIZE];
    int baseStocks;
    int stocks;
    double price;
    char dateAdded[30];
    char lastUpdated[30];
    char id[ID_LENGTH];
};

// structure of an individual node in a linked list
struct Node
{
    struct Item data;       // contains the Item
    struct Node *next;      // a pointer that points to the next item
};

// function prototypes
void printLinkedlist(const struct Node *head);
void addItem(struct Node **head);
void deleteItem(struct Node **head);
int getListSize(const struct Node *head);
void updateItem(struct Node **head);
void searchItem(struct Node **head);
void sellItem(struct Node **head);
void freeLinkedList(struct Node **head);
void addTestItem(struct Node **head, char name[], int stocks, double price);
char* getCurrentTime();
char* getCurrentDate();
char *generateId(char placeholder[]);
void printHelp();
void printActions();

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

void sellItem(struct Node **head)
{
    system("cls");
    // TODO: fix an unknown issue here
    struct Node *current = *head;
    char itemId[ID_LENGTH];
    int quantity;
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
    scanf("%d", &quantity);

    while (current != NULL) {
        if (strcmp(current->data.id, itemId) == 0) {
            current->data.stocks -= quantity;
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

void searchItem(struct Node **list)
{
    system("cls");
    printf("Searching an item.\n\n");
    printLinkedlist(*list);

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

void addItem(struct Node **head)
{
    system("cls");
    printf("Adding an item.\n\n");
    // create a new instance of the Item struct
    struct Item newItem;

    scanf("%c");
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

    printf("Enter the price of item: \n>>> ");
    scanf("%lf", &newItem.price);

    // get the current date and time
    sprintf(newItem.dateAdded, "%s | %s", getCurrentDate(), getCurrentTime());
    sprintf(newItem.lastUpdated, "%s | %s", getCurrentDate(), getCurrentTime());

    generateId(newItem.id);

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

void deleteItem(struct Node **head)
{
    system("cls");
    printf("Deleting an item.\n\n");
    printLinkedlist(*head);
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
        deleted = *head;
        *head = current->next;
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

        // if we did find it and break out the loop, proceed here
        deleted = current->next;
        // now that were at the Node right before the Node TO BE DELETED,
        // the (current->next) is the TO BE DELETED so we simply overwrite it
        // by setting it to point to the next Node of the Node TO BE DELETED
        current->next = current->next->next;
    }
    // free the memory
    free(deleted);
    // after deleting the item, reprint the list
    printLinkedlist(*head);
    printf("\n\nItem deleted successfully!");
    printf("\nEnter 'b' to go back.");
}

void updateItem(struct Node **head)
{
    system("cls");
    printf("Updating an item.\n\n");
    printLinkedlist(*head);

    int addedStocks;
    int toUpdate;
    // this will hold the index of item to be deleted
    char idToDelete[ID_LENGTH];

    // prompt the user for some details
    printf("\n\nEnter the ID of the item: \n>>> ");
    scanf("%s", idToDelete);

    printf("\nOptions: [ 1. name | 2. stocks | 3. price ]");
    printf("\nWhat do you want to update?: \n>>> ");
    scanf("%d", &toUpdate);

    // we create a current variable so that we don't manipulate the pointer of the head
    struct Node *current = *head;
    int j = 1;
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
            // TODO: use fgets()
            scanf("%c");
            fgets(current->data.name, NAME_SIZE, stdin);

            // clear the newline character from fgets()
            size_t len = strlen(current->data.name);
            if (len > 0 && current->data.name[len - 1] == '\n')
                current->data.name[len - 1] = '\0';
            // scanf("%s", current->data.name);
            break;
        case 2:
            printf("\nEnter the amount of stocks added in %s: \n>>> ", current->data.name);
            scanf("%d", &addedStocks);
            current->data.stocks += addedStocks;
            current->data.baseStocks = current->data.stocks;
            break;
        case 3:
            printf("\nEnter an updated price of %s: \n>>> ", current->data.name);
            scanf("%lf", &current->data.price);
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

char* getCurrentTime() {
    time_t now;
    struct tm *localTime;
    // the string to store the final output
    static char timeString[12];

    // get the time using the <time.h> library and storing it in now variable
    time(&now);
    // get the local time
    localTime = localtime(&now);

    // extract the datas from the struct tm
    int hour = localTime->tm_hour;
    int minute = localTime->tm_min;
    char period[3]; // AM or PM

    // converting it into AM/PM instead of military time
    if (hour >= 12) {
        strcpy(period, "PM");
        if (hour > 12) {
            hour -= 12;
        }
    } else {
        strcpy(period, "AM");
        if (hour == 0) {
            hour = 12;
        }
    }

    // format the time string
    sprintf(timeString, "%02d:%02d %s", hour, minute, period);

    return timeString;
}

char* getCurrentDate()
{
    time_t currentTime;
    struct tm *localTime;
    static char dateString[11];

    time(&currentTime);
    localTime = localtime(&currentTime);

    // extract the datas from the struct tm
    int year = localTime->tm_year + 1900;
    int month = localTime->tm_mon + 1;
    int day = localTime->tm_mday;

    // format the date string
    sprintf(dateString, "%d-%02d-%02d", year, month, day);

    return dateString;
}

char *generateId(char placeholder[])
{
    char chars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char nums[] = "1234567890";

    placeholder[0] = '\0';

    // get 4 random characters
    for (int i = 0; i < 4; i++)
    {
        char temp[2] = {chars[rand() % strlen(chars)], '\0'};
        strcat(placeholder, temp);
    }

    // get 4 random numbers
    for (int i = 0; i < 4; i++)
    {
        char temp[2] = {nums[rand() % strlen(nums)], '\0'};
        strcat(placeholder, temp);
    }
}

// for development purposes only
void addTestItem(struct Node **head, char name[], int stocks, double price)
{
    struct Item newItem;

    strcpy(newItem.name, name);
    newItem.stocks = stocks;
    newItem.baseStocks = stocks;
    newItem.price = price;

    sprintf(newItem.dateAdded, "%s | %s", getCurrentDate(), getCurrentTime());
    sprintf(newItem.lastUpdated, "%s | %s", getCurrentDate(), getCurrentTime());

    generateId(newItem.id);

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
