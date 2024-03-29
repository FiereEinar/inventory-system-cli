#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// I'm using linked list because it makes that list management easier.
// adding, deleting, updating, etc. is so much easier than using traditional arrays
// structure of an individual item
struct Item
{
    char name[20];
    int stocks;
    double price;
    char dateAdded[30];
    char lastUpdated[30];
};

// structure of an individual node in a linked list
struct Node
{
    // the data property of a node will contain the Item
    struct Item data;
    // the next property of a node is a pointer that points to the next node
    struct Node *next;
};

// function prototypes
void printLinkedlist(const struct Node *p);
void addItem(struct Node **head);
void deleteItem(struct Node **head);
int getListSize(const struct Node *head);
void updateItem(struct Node **head);
void searchItem(const struct Node **head);
void addTestItem(struct Node **head, char name[], int stocks, double price);
char* getCurrentTime();
char* getCurrentDate();
void printHelp();

int main()
{
    // the head is initially set to NULL
    struct Node *head = NULL;
    // action is the placeholder for determining the action that the user wants to take
    char action[15];
    printf("\nType 'help' to see all available commands.");

    printf("\nAdding test items for development purposes only.");
    addTestItem(&head, "pencil", 25, 10);
    addTestItem(&head, "paper", 50, 40);
    addTestItem(&head, "scissors", 15, 56);
    addTestItem(&head, "ballpen", 18, 10);
    addTestItem(&head, "scraper", 46, 12);
    addTestItem(&head, "comb", 4, 46);

    do {
        printf("\n\nCommand: ");
        scanf("%s", action);

        // here we just examine the action that the user wants to take
        // on each action taken we provide an address to the head that way the functions can access it or manipulate it
        if (strcmp(action, "help") == 0) printHelp();
        if (strcmp(action, "view") == 0) printLinkedlist(head);
        if (strcmp(action, "add") == 0) addItem(&head);
        if (strcmp(action, "delete") == 0) deleteItem(&head);
        if (strcmp(action, "update") == 0) updateItem(&head);
        if (strcmp(action, "search") == 0) searchItem(&head);
    } while (strcmp(action, "exit") != 0);

    return 0;
}

void searchItem(const struct Node **list)
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

    printf("\n\nEnter a keyword: ");
    scanf("%s", searchTerm);

    printf("\nSearch: %s", searchTerm);

    // we traverse the list and on each iteration, check if searchTerm is a substring of the current data name
    while(current != NULL) {
        // strstr() is a function in <string.h> that will check if a string is a substring of another string
        if(strstr(current->data.name, searchTerm) != NULL) {
            // if true we create a new instance of Node with the same data
            struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
            newNode->data = current->data;
            newNode->next = NULL;

            // then store it in the results, result is also a linked list just like the head in main()
            // this first checks if this is the first elements being stored
            if(results == NULL) {
                results = newNode;
                // we keep track of the tail of the list so that we don't have to loop our way there
                last = results;
            } else {
                // if its not the first Node, go to the tail and add it there
                last->next = newNode;
                // keep track of tail
                last = newNode;
            }
        }
        // go to next node
        current = current->next;
    }
    // print all the result
    printLinkedlist(results);
    // free the memory of results
    freeLinkedList(results);
}

void addItem(struct Node **head)
{
    system("cls");
    printf("Adding an item.\n\n");
    // create a new instance of the Item struct
    struct Item newItem;

    // prompt the user with all the data and store it in that newItem structure
    printf("Enter the name of item: ");
    scanf("%s", newItem.name);

    printf("Enter the stocks of item: ");
    scanf("%d", &newItem.stocks);

    printf("Enter the price of item: ");
    scanf("%lf", &newItem.price);

    // get the current date and time
    sprintf(newItem.dateAdded, "%s | %s", getCurrentDate(), getCurrentTime());
    sprintf(newItem.lastUpdated, "%s | %s", getCurrentDate(), getCurrentTime());

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
}

void deleteItem(struct Node **head)
{
    system("cls");
    printf("Deleting an item.\n\n");
    printLinkedlist(*head);
    // this will hold the index of item to be deleted
    int index;

    printf("\n\nEnter the index of the item you want to delete: ");
    scanf("%d", &index);

    // if the number entered by user is not in the list we give them an error
    if (index > getListSize(*head) || index <= 0)
    {
        printf("\nError: Item does not exist. Please try again");
        return;
    }

    // we create a current variable so that we don't manipulate the pointer of the head
    struct Node *current = *head;
    // we keep track of the deleted Node to free its memory since it was dynamically allocated
    struct Node *deleted = NULL;
    int j = 1;

    // if the user is trying to delete the first item, we simply set the head to point to the next node
    if (index == 1) {
        deleted = *head;
        *head = current->next;
    } else {
        // to delete a Node, we traverse to the Node right before the Node TO BE DELETED
        // this is why the while loop is (index - 1)
        while(index - 1 != j)
        {
            current = current->next;
            j++;
        }
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
}

void updateItem(struct Node **head)
{
    system("cls");
    printf("Updating an item.\n\n");
    printLinkedlist(*head);

    int toUpdate, index;

    // prompt the user for some details
    printf("\n\nEnter the index of the item: ");
    scanf("%d", &index);

    printf("\nOptions: [ 1. name | 2. stocks | 3. price ]");
    printf("\nWhat do you want to update?: ");
    scanf("%d", &toUpdate);

    // we create a current variable so that we don't manipulate the pointer of the head
    struct Node *current = *head;
    int j = 1;
    // traverse to the Node/item that the user wants to update
    while(index != j)
    {
        current = current->next;
        j++;
    }

    // after that, we simply determine what the user wants to update and prompt the user for new data
    switch(toUpdate) {
    case 1:
        printf("\nEnter a new name of %s: ", current->data.name);
        scanf("%s", current->data.name);
        break;
    case 2:
        printf("\nEnter an updated stock of %s: ", current->data.name);
        scanf("%d", &current->data.stocks);
        break;
    case 3:
        printf("\nEnter an updated price of %s: ", current->data.name);
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

// for development purposes only
void addTestItem(struct Node **head, char name[], int stocks, double price)
{
    struct Item newItem;

    strcpy(newItem.name, name);
    newItem.stocks = stocks;
    newItem.price = price;

    sprintf(newItem.dateAdded, "%s | %s", getCurrentDate(), getCurrentTime());
    sprintf(newItem.lastUpdated, "%s | %s", getCurrentDate(), getCurrentTime());

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
void printLinkedlist(const struct Node *p)
{
    int i = 1;
    printf("\nItem:\t\t\tStocks:\t\tPrice:\t\tDate Added:\t\t\tLast Updated:");
    // here we are using the same logic that we've used in the addItem function,
    // but instead we are printing each the contents.
    while (p != NULL)
    {
        printf
        (
            "\n%d. %-20s\t%d\t\tP%.2lf\t\t%s\t\t%s",
            i, p->data.name, p->data.stocks, p->data.price, p->data.dateAdded, p->data.lastUpdated
        );
        p = p->next;
        i++;
    }
}

void freeLinkedList(struct Node *head) {
    struct Node *current = head;
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
