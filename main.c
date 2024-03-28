#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// structure of an individual item
struct Item
{
    char name[20];
    int stocks;
    double price;
};

// structure of an individual node in a linked list
struct Node
{
    // the data property of a node will contain the Item
    struct Item data;
    // the next property of a node is a pointer that points to the next node
    struct Node *next;
};

void printLinkedlist(const struct Node *p);
void addItem(struct Node **head);
void deleteItem(struct Node **head);
int getListSize(const struct Node *head);
void addTestItem(struct Node **head, char name[], int stocks, double price);
char* getCurrentTime();
void printHelp();

int main()
{
    // the head is initially set to NULL
    struct Node *head = NULL;
    // x is the placeholder for determining the action that the user wants to take
    char x[15];
    printf("\nType 'help' to see all available commands.");

    addTestItem(&head, "pencil", 25, 10);
    addTestItem(&head, "paper", 50, 40);
    addTestItem(&head, "scissors", 15, 56);

    do {
        printf("\n\nEnter your action: ");
        scanf("%s", x);

        // here we just examine the action that the user wants to take
        if (strcmp(x, "help") == 0) printHelp();
        if (strcmp(x, "read") == 0) printLinkedlist(head);
        if (strcmp(x, "add") == 0) addItem(&head);
        if (strcmp(x, "delete") == 0) deleteItem(&head);
    } while (strcmp(x, "exit") != 0);

    return 0;
}

void deleteItem(struct Node **head)
{
    system("cls");

    // this will hold the index of item to be deleted
    int index;
    printLinkedlist(*head);

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
    int j = 1;

    // if the user is trying to delete the first item, we simply set the head to point to the next node
    if (index == 1) {
        *head = current->next;
    } else {
        // to delete a Node, we traverse to the Node right before the Node TO BE DELETED
        // this is why the while loop is (index - 1)
        while(index - 1 != j)
        {
            current = current->next;
            j++;
        }
        // now that were at the Node right before the Node TO BE DELETED,
        // the (current->next) is the TO BE DELETED so we simply overwrite it
        // by setting it to point to the next Node of the Node TO BE DELETED
        current->next = current->next->next;
    }
    // after deleting the item, reprint the list
    printf("\nItem deleted successfully!");
    printLinkedlist(*head);
}

int getListSize(const struct Node *head)
{
    int size = 0;

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
    printf("\n1. help \t print all available commands.");
    printf("\n2. exit \t exit out the operation.");
    printf("\n3. read \t print all items in the inventory.");
    printf("\n4. add \t\t add an item to inventory.");
    printf("\n4. delete \t delete an item to inventory.");
}

char* getCurrentTime()
{
    time_t now;
    struct tm *localTime;
    static char timeString[9]; // Hour:Minute:Seconds

    time(&now);
    localTime = localtime(&now);

    sprintf(timeString, "%02d:%02d:%02d", localTime->tm_hour, localTime->tm_min, localTime->tm_sec);

    return timeString;
}

void addItem(struct Node **head)
{
    system("cls");
    // create a new instance of the Item struct
    struct Item newItem;

    // prompt the user with all the data and store it in that newItem structure
    printf("Enter the name of item: ");
    scanf("%s", newItem.name);

    printf("Enter the stocks of item: ");
    scanf("%d", &newItem.stocks);

    printf("Enter the price of item: ");
    scanf("%lf", &newItem.price);

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
        // so we use it to check wether of not we have reached the end of the list
        while(current->next != NULL)
        {
            // we traverse the list by simply setting our current pointer to point to the next Node
            // this will continue until the next property in NULL and that means we have reach the end
            current = current->next;
        }
        // after we reached the end, we simply set the next pointer to point to our newly created Node
        current->next = newNode;
    }
    printf("\nItem added!");
    printf("\nType 'read' to view inventory");
}

void addTestItem(struct Node **head, char name[], int stocks, double price)
{
    struct Item newItem;

    strcpy(newItem.name, name);
    newItem.stocks = stocks;
    newItem.price = price;

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
    printf("\nType 'read' to view inventory");
}

// print the linked list value
void printLinkedlist(const struct Node *p)
{
    system("cls");
    int i = 1;
    printf("\nItem:\t\t\tStocks:\t\tPrice:");
    // here we are using the same logic that we've used in the addItem function,
    // but instead we are printing each the contents.
    while (p != NULL)
    {
        printf("\n%d. %-20s\t%d\t\tP%.2lf", i, p->data.name, p->data.stocks, p->data.price);
        p = p->next;
        i++;
    }
}
